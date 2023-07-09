#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <regex.h>

#define IP_ADDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
#define IP_ADDR_W_CIDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/([0-9]|[12][0-9]|3[0-2])$"

typedef int bool_t;

static u_int32_t __dotted_decimal_to_32bit(u_int8_t first, u_int8_t second, u_int8_t third, u_int8_t fourth){
    return (first << 24) | (second << 16) | (third << 8) | (fourth);
}

static u_int32_t __cidr_to_32bit(int cidr){
    return (u_int32_t)(~0) << (32 - cidr);
    // I hope the ~0 turns into 32 ones
}

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

    if(*arg++ == '/'){
        for(digit_count = 0; *arg; arg++){
            buffer[digit_count++] = *arg;  
        }
        buffer[digit_count] = 0;
        return atoi(buffer); // Returns subnet
    } else {

    }
    return -1;
}

int process_input_ip_and_mask(char* arg1, char* arg2, u_int32_t* ip_addr, u_int32_t* new_subnet, u_int32_t* old_subnet){
    if(arg2 == NULL){
        // Case 1
        if(__invalid_ip_addr(arg1, IP_ADDR_W_CIDR_REGEX)){
            fprintf(stderr, "Invalid IP address with CIDR mask\n");
            return 2;
        }

        // Get ip address and save it in ip_addr
        u_int8_t ip_array[4];

        int new_subnet_cidr = __ip_string_to_uint32(arg1, ip_array);
        *ip_addr = __dotted_decimal_to_32bit(ip_array[0], ip_array[1], ip_array[2], ip_array[3]);

        *new_subnet = __cidr_to_32bit(new_subnet_cidr);

        // Get old subnet and check that new subnet < old subnet
        if(ip_array[0] <= 126){
            if(new_subnet_cidr < 8){
                fprintf(stderr, "Invalid subnet mask\n");
                return 3;
            }
            *old_subnet = __cidr_to_32bit(8);
        } else if(ip_array[0] >= 128 && ip_array[0] <= 191){
            if(new_subnet_cidr < 16){
                fprintf(stderr, "Invalid subnet mask\n");
                return 3;
            }
            *old_subnet = __cidr_to_32bit(16);
        } else if(ip_array[0] >= 192 && ip_array[0] <= 223){
            if(new_subnet_cidr < 24){
                fprintf(stderr, "Invalid subnet mask\n");
                return 3;
            }
            *old_subnet = __cidr_to_32bit(24);
        } else {
            fprintf(stderr, "Invalid IP address with CIDR mask\n");
            return 2;
        }
    } else {
        // Case 2 or 3
        if(__invalid_ip_addr(arg1, IP_ADDR_REGEX)){
            fprintf(stderr, "Invalid IP address\n");
            return 2;
        }
        if(__invalid_ip_addr(arg2, IP_ADDR_REGEX)){
            fprintf(stderr, "Invalid dotted decimal mask\n");
            return 3;
        }

        // Case 2
        // Case 3
        


    }

    return 0;
}