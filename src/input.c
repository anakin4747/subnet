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

static int __cidr_from_class(char class){
    // 'A' = 8
    // 'B' = 16
    // 'C' = 24
    return (int)((class - 'A' + 1) * 8);
}

// Regex validation function
static bool_t __invalid_ip_addr(char* ip_addr, const char* ip_exp){
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

// Regex validation function
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

// Validation function
static bool_t __invalid_32bit_mask(u_int32_t mask_32){
    // Would love a replacement thats just digital logic instead so that its shorter
    int i;
    for(i = 0; (mask_32 & 1) == 0; mask_32 >>= 1, i++){ }
    for(; (mask_32 & 1); mask_32 >>= 1, i++){ }

    return i == 32 ? 0 : 1;
}

// Validation function
static bool_t __invalid_network_addr(u_int32_t net_addr, u_int32_t new_mask){
    return (net_addr & ~new_mask);
}

// Private function
// Cases 4 & 5
// Returns new_subnet_cidr if possible otherwise returns -1
static int __process_max_expression(char* arg, int old_mask_cidr){
    char buffer[11];
    int i, max_number, new_subnet_cidr;

    // Process digits in front of s or h
    for(i = 0; isdigit(arg[i]); i++){
        buffer[i] = arg[i];
    }
    buffer[i] = '\0'; // NULL terminate buffer

    if(i == 0){ // If no digit
        max_number = 1; // Would this make sense as an input? just s or h
    } else {
        max_number = atoi(buffer);
    }
    switch (arg[i]){
        case 's':
            int borrowed_bits;
            // Find the number of borrowed_bits needed for that many subnets
            for(borrowed_bits = 1; (pow(2, borrowed_bits) < max_number); borrowed_bits++){ }
            new_subnet_cidr = old_mask_cidr + borrowed_bits;
            return new_subnet_cidr > 30 ? -1 : new_subnet_cidr;
            break;
        case 'h':
            int host_bits;
            // Find the number of host_bits needed for that many host per subnet
            for(host_bits = 2; ((pow(2, host_bits) - 2) < max_number); host_bits++){ }
            new_subnet_cidr = 32 - host_bits;
            return new_subnet_cidr <= old_mask_cidr ? -1 : new_subnet_cidr;
            break;
        default:
            break;
    }
    return -1;
}

// Private function
// Converts a valid IP string to an array of 4 bytes (ip_array)
// Saves class of IP based off first octet in char* class
// Saves Subnet in subnet_cidr if CIDR notation is present, -1 otherwise
static int __ip_string_to_array(char* arg, u_int8_t* ip_array, char* class, int* subnet_cidr){
    int digit_count = 0;    
    int octet_count = 0;    
    int subnet_input;
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

    // Prevents seg fault and reduces need for boolean arg
    if(class != NULL){
        if(*arg++ == '/'){
            for(digit_count = 0; *arg; arg++){
                buffer[digit_count++] = *arg;  
            }
            buffer[digit_count] = 0;
            subnet_input = atoi(buffer); 
            if(subnet_input > 30){
                fprintf(stderr, "Invalid CIDR subnet mask\n");
                return -3;
            }
            *subnet_cidr = subnet_input;
        } else {
            *subnet_cidr = -1;
        }

        if(ip_array[0] <= 126){
            *class = 'A';
        } else if(ip_array[0] >= 128 && ip_array[0] <= 191){
            *class = 'B';
        } else if(ip_array[0] >= 192 && ip_array[0] <= 223){
            *class = 'C';
        } else {
            // Check if the dotted decimal string is an IP address instead of a mask
            fprintf(stderr, "Invalid IP address based on first octet\n");
            return -2;
        }
    }
    return 0;
}

// Public function
// Main interface for processing command line arguments for all cases, and saving 
int process_input_ip_and_masks(char* arg1, char* arg2, u_int32_t* ip_addr, u_int32_t* new_subnet, u_int32_t* old_subnet){

    if(__invalid_ip_addr(arg1, IP_ADDR_REGEX) && __invalid_ip_addr(arg1, IP_ADDR_W_CIDR_REGEX)){
        fprintf(stderr, "Invalid IP address\n");
        return 2;
    }

    // Process arg1 for IP address and class
    u_int8_t ip_array[4];
    char class;
    int result, input_cidr, old_subnet_cidr, new_subnet_cidr;

    // Convert ip address from string to array 
    // Save class & returning error if class D or E
    // Save CIDR or -1 in input_cidr & returning error if CIDR > 30
    result = __ip_string_to_array(arg1, ip_array, &class, &input_cidr);
    if(result){
        return result;
    }
    // Set IP address
    *ip_addr = dotted_decimal_to_32bit(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);

    // Set old_subnet
    if(input_cidr == -1 || arg2 == NULL){
        // Cases 1, 2, 4, 6
        old_subnet_cidr = __cidr_from_class(class);
    } 
    if(input_cidr != -1 && arg2 != NULL){
        // Cases 3, 5, 7
        old_subnet_cidr = input_cidr;
    }
    *old_subnet = cidr_to_32bit(old_subnet_cidr);

    // Set new_subnet
    if(arg2 == NULL){
        // Case 1 
        new_subnet_cidr = input_cidr;
        *new_subnet = cidr_to_32bit(new_subnet_cidr);
    }
    if(arg2 != NULL && !__invalid_ip_addr(arg2, IP_ADDR_REGEX)){
        // If arg2 is a valid IP addr just based on regex
        // Case 2 or 3

        u_int8_t new_subnet_array[4];
        result = __ip_string_to_array(arg2, new_subnet_array, NULL, NULL);
        if(result){
            return result;
        }
        *new_subnet = dotted_decimal_to_32bit(new_subnet_array[0], new_subnet_array[1], new_subnet_array[2], new_subnet_array[3]);
        new_subnet_cidr = convert_32_mask_to_cdir(*new_subnet);
        
        if(__invalid_32bit_mask(*new_subnet)){
            fprintf(stderr, "Invalid dotted decimal mask\n");
            return 3;
        }
    }
    if(arg2 != NULL && !__invalid_max_s_or_h(arg2)){
        // If arg2 is a valid max expression
        // Case 4 or 5
        new_subnet_cidr = __process_max_expression(arg2, old_subnet_cidr);
        if(new_subnet_cidr == -1){
            fprintf(stderr, "Invalid max expression\n");
            return 4;
        }
        *new_subnet = cidr_to_32bit(new_subnet_cidr);
    }
    
    // Catch invalid argv[2]
    if(arg2 != NULL && __invalid_ip_addr(arg2, IP_ADDR_REGEX) && __invalid_max_s_or_h(arg2)){
        fprintf(stderr, "Invalid second argument\n");
        return 6;
    }


    // Validate new mask
    if(old_subnet_cidr > new_subnet_cidr){
        fprintf(stderr, "Invalid CIDR mask, smaller than old mask\n"); 
        return 3;

    } else if(old_subnet_cidr == new_subnet_cidr){
        fprintf(stderr, "Invalid CIDR mask, same as old mask\n");
        return 3;
    }

    // Validate network address
    if(__invalid_network_addr(*ip_addr, *new_subnet)){
        fprintf(stderr, "Invalid network address\n");
        return 2;
    }

    return 0;
}