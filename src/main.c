#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "subnet_options.h"
#include "input.h"

#define MAX_ARG_SIZE 4

int main(int argc, char** argv){

    if(argc == 1){
        printf("Wrong usage\n");
        exit(1);
    } else if(argc >= MAX_ARG_SIZE){
        printf("Too many arguments\n");
        exit(1);
    } else if(argc == 2){
        // subnet [Classless subnet block with CIDR mask]
        u_int32_t ip_addr, new_subnet, old_subnet, result;
        result = process_input_ip(argv[1], NULL, &ip_addr, &new_subnet, &old_subnet);
        if(result){
            exit(result);
        }
        printf("IP address as 32 bits: %u\n"
               "New subnet mask 32 bits: %u\n"
               "Old subnet mask 32 bits: %u\n",
               ip_addr, new_subnet, old_subnet);
        // subnet_classless_cidr_mask(argv[1]);
    } else if(argc == 3){
        int length = strlen(argv[2]);

        switch (argv[2][length - 1]){
            case 'h':
            case 's':
                // subnet [Subnet block] [0-9]+(s|h)
                // subnet_max(argv[1], argv[2]);
                break;
            default:
                // subnet [Classless IP address] [Dotted decimal mask]
                // subnet_classless_dd_mask(argv[1], argv[2]);
                break;
        }
    } else {
        // subnet [Subnet block] s:i s:i s:i
        // subnet_expressions(argv + 2);
    }
    return 0;
}

/*

exit codes
1 - exited in the main from wrong number of arguments
2 - exited from invalid IP block
3 - exited from invalid subnet mask

*/