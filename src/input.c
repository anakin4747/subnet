#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <regex.h>

#include "helper_functions.h"

#define IP_ADDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define IP_ADDR_W_CIDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/([0-9]|[12][0-9]|3[0-2])$"
#define MAX_EXPRESSION_REGEX "^[0-9]{0,10}(h|s)$"

typedef int bool_t;

// Validation function
static bool_t __invalid_ip_addr(char* ip_addr, const char* ip_exp){
    // Save space on the stack I guess
    regex_t* regex = (regex_t*)malloc(sizeof(regex_t));

    // Compile regex
    if(regcomp(regex, ip_exp, REG_EXTENDED)){
        fprintf(stderr, "Could not compile REGEX\n");
        free(regex);
        return 1;
    }

    // Perform regex comparison
    if(regexec(regex, ip_addr, 0, NULL, 0)){
        free(regex);
        return 1;
    }

    free(regex);
    return 0;
}

// Validation function
static bool_t __invalid_32bit_mask(u_int32_t mask_32){
    int i;
    for(i = 0; (mask_32 & 1) == 0; mask_32 >>= 1, i++){ }
    for(; (mask_32 & 1); mask_32 >>= 1, i++){ }

    return i == 32 ? 0 : 1;
}

// Validation function
static bool_t __invalid_network_addr(u_int32_t net_addr, u_int32_t new_mask){
    return (net_addr & ~new_mask);
}

// Validation function
static bool_t __invalid_max_s_or_h(char* arg){
    regex_t* regex = (regex_t*)malloc(sizeof(regex_t));

    // Compile regex
    if(regcomp(regex, MAX_EXPRESSION_REGEX, REG_EXTENDED)){
        fprintf(stderr, "Could not compile REGEX\n");
        free(regex);
        return 1;
    }

    // Perform regex comparison
    if(regexec(regex, arg, 0, NULL, 0)){
        free(regex);
        return 1;
    }

    free(regex);
    return 0;
}

// Private function
// Cases 4 & 5
static int __process_max_expression(char* arg, int old_mask_cidr){
    char buffer[11];
    int i, max_number;

    for(i = 0; isdigit(arg[i]); i++){
        buffer[i] = arg[i];
    }
    buffer[i] = '\0'; // i or i + 1?

    if(i == 0){
        max_number = 1; // Would this make sense as an input? just s or h
    } else {
        max_number = atoi(buffer);
    }
    switch (arg[i]){
        case 's':
            int borrowed_bits;
            for(borrowed_bits = 1; (pow(2, borrowed_bits) < max_number); borrowed_bits++){ }
            return old_mask_cidr + borrowed_bits;
            break;
        case 'h':
            int host_bits;
            for(host_bits = 2; ((pow(2, host_bits) - 2) < max_number); host_bits++){ }
            return 32 - host_bits;
            break;
        default:
            break;
    }
    return -1;
}

// Private function
static int __ip_string_to_uint32(char* arg, u_int8_t* ip_array){
    int digit_count = 0;    
    int octet_count = 0;    
    char buffer[4]; 
    // Iterate over the input   
    for(; *arg && *arg != '/'; arg++){   
        if(isdigit(*arg)){ 
        // If it is a digit append it to the buffer 
            buffer[digit_count++] = *arg;  
        } else {    
        // If not, null terminate buffer and reset digit_count  
            buffer[digit_count] = 0;    
            digit_count = 0;    

            // Store octet in array 
            ip_array[octet_count++] = atoi(buffer); 
        }   
    }   
    // Need to null terminate or else bug
    buffer[digit_count] = 0;    
    ip_array[octet_count] = atoi(buffer); 

    if(*arg++ == '/'){
        for(digit_count = 0; *arg; arg++){
            buffer[digit_count++] = *arg;  
        }
        buffer[digit_count] = 0;
        return atoi(buffer); // Returns subnet
    }
    return -1;
}

// Get old subnet and check that new subnet < old subnet
// This can be used for cases 1, 2, 4, and 6
// Private function
static int __set_classful_mask(u_int32_t net_addr, int new_subnet_cidr, u_int32_t* old_subnet){
    u_int8_t first_octet = net_addr >> 24;

    if(first_octet <= 126){
        if(new_subnet_cidr < 8){
            fprintf(stderr, "Invalid subnet mask\n");
            return 3;
        }
        *old_subnet = cidr_to_32bit(8);
    } else if(first_octet >= 128 && first_octet <= 191){
        if(new_subnet_cidr < 16){
            fprintf(stderr, "Invalid subnet mask\n");
            return 3;
        }
        *old_subnet = cidr_to_32bit(16);
    } else if(first_octet >= 192 && first_octet <= 223){
        if(new_subnet_cidr < 24){
            fprintf(stderr, "Invalid subnet mask\n");
            return 3;
        }
        *old_subnet = cidr_to_32bit(24);
    } else {
        fprintf(stderr, "Invalid IP address with CIDR mask\n");
        return 2;
    }
    if(__invalid_network_addr(net_addr, cidr_to_32bit(new_subnet_cidr))){
        fprintf(stderr, "Invalid network address\n");
        return 2;
    }
    return 0;
}

// This could use some refactoring once more cases are working
// Public function
int process_input_ip_and_masks(char* arg1, char* arg2, u_int32_t* ip_addr, u_int32_t* new_subnet, u_int32_t* old_subnet){
    if(arg2 == NULL){
        // Case 1
        if(__invalid_ip_addr(arg1, IP_ADDR_W_CIDR_REGEX)){
            fprintf(stderr, "Invalid IP address with CIDR mask\n");
            return 2;
        }

        // Get ip address and save it in ip_addr
        u_int8_t ip_array[4];

        int new_subnet_cidr = __ip_string_to_uint32(arg1, ip_array);
        *ip_addr = dotted_decimal_to_32bit(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);

        *new_subnet = cidr_to_32bit(new_subnet_cidr);

        // Set old subnet from a classful IP address and propagate error codes
        return __set_classful_mask(*ip_addr, new_subnet_cidr, old_subnet);

    } else if(index(arg2, 's') || index(arg2, 'h')){

        if(__invalid_ip_addr(arg1, IP_ADDR_REGEX) && __invalid_ip_addr(arg1, IP_ADDR_W_CIDR_REGEX)){
            fprintf(stderr, "Invalid IP address\n");
            return 2;
        }

        if(__invalid_max_s_or_h(arg2)){
            fprintf(stderr, "Invalid max expression\n");
            return 4;
        }
        u_int8_t ip_array[4];
        int old_subnet_cidr = __ip_string_to_uint32(arg1, ip_array);
        *ip_addr = dotted_decimal_to_32bit(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);

        // Process second argument and return new subnet
        int new_subnet_cidr = __process_max_expression(arg2, old_subnet_cidr);

        if(new_subnet_cidr < 0){
            fprintf(stderr, "Error processing max expression\n");
            return 4;
        }

        *new_subnet = cidr_to_32bit(new_subnet_cidr);

        if(old_subnet_cidr == -1){
            // Case 4
            // Set old subnet from a classful IP address and propagate error codes
            return __set_classful_mask(*ip_addr, new_subnet_cidr, old_subnet);
        } else {
            // Case 5
            if(old_subnet_cidr > new_subnet_cidr){
                fprintf(stderr, "Invalid subnet mask\n");
                return 3;
            }
            if(__invalid_network_addr(*ip_addr, *new_subnet)){
                fprintf(stderr, "Invalid network address\n");
                return 2;
            }
            *old_subnet = cidr_to_32bit(old_subnet_cidr);
        }

    } else {
        // Case 2 or 3
        if(__invalid_ip_addr(arg1, IP_ADDR_REGEX) && __invalid_ip_addr(arg1, IP_ADDR_W_CIDR_REGEX)){
            fprintf(stderr, "Invalid IP address\n");
            return 2;
        }
        if(__invalid_ip_addr(arg2, IP_ADDR_REGEX)){
            fprintf(stderr, "Invalid dotted decimal mask\n");
            return 3;
        }


        u_int8_t ip_array[4];
        int old_subnet_cidr = __ip_string_to_uint32(arg1, ip_array);
        *ip_addr = dotted_decimal_to_32bit(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);

        u_int8_t new_subnet_array[4];
        __ip_string_to_uint32(arg2, new_subnet_array);
        *new_subnet = dotted_decimal_to_32bit(new_subnet_array[0], new_subnet_array[1], new_subnet_array[2], new_subnet_array[3]);
        
        if(__invalid_32bit_mask(*new_subnet)){
            fprintf(stderr, "Invalid dotted decimal mask\n");
            return 3;
        }

        // New subnet uint32 to cidr
        int new_subnet_cidr = convert_32_mask_to_cdir(*new_subnet);

        if(old_subnet_cidr == -1){
            // Case 2
            // Set old subnet from a classful IP address and propagate error codes
            return __set_classful_mask(*ip_addr, new_subnet_cidr, old_subnet);
        } else {
            // Case 3
            if(old_subnet_cidr > new_subnet_cidr){
                fprintf(stderr, "Invalid subnet mask\n");
                return 3;
            }
            if(__invalid_network_addr(*ip_addr, *new_subnet)){
                fprintf(stderr, "Invalid network address\n");
                return 2;
            }
            *old_subnet = cidr_to_32bit(old_subnet_cidr);
        }
    }

    return 0;
}