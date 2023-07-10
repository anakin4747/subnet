#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subnet_block.h"
#include "input.h"

#define MAX_ARG_SIZE 4

int main(int argc, char** argv){

    u_int32_t ip_addr, new_subnet, old_subnet, result;

    if(argc == 1){
        printf("Wrong usage\n");
        exit(1);
    } else if(argc >= MAX_ARG_SIZE){
        printf("Too many arguments\n");
        exit(1);
    } else if(argc == 2){
        // Case 1:
        // subnet [Network address]/[New CIDR mask]
        result = process_input_ip_and_mask(argv[1], NULL, &ip_addr, &new_subnet, &old_subnet);
        if(result){
            exit(result);
        }
        
        struct subnet_block_t* subnet_block = subnet_block_malloc();

        subnet_block_constructor(subnet_block, ip_addr, new_subnet, old_subnet);

        subnet_block_print(subnet_block);
        subnet_block_print_address_ranges(subnet_block);

        subnet_block_destructor(subnet_block);
        free(subnet_block);

        
    } else if(argc == 3){
        int length = strlen(argv[2]);
        char last_char;

        switch (last_char = argv[2][length - 1]){
            case 'h':
            case 's':
                // Case 4:
                // subnet [Network address] [0-9]+(s|h)
                // Case 5:
                // subnet [Network address]/[Old CIDR mask] [0-9]+(s|h)
                result = process_input_ip_and_mask(argv[1], NULL, &ip_addr, &new_subnet, &old_subnet);
                if(result){
                    exit(result);
                }
                break;
            default:

                // Case 2:
                // subnet [Network address] [Dotted decimal subnet mask]
                // Case 3:
                // subnet [Network address]/[Old CIDR mask] [New dotted decimal subnet mask]
                result = process_input_ip_and_mask(argv[1], argv[2], &ip_addr, &new_subnet, &old_subnet);
                if(result){
                    exit(result);
                }

                struct subnet_block_t* subnet_block = subnet_block_malloc();

                subnet_block_constructor(subnet_block, ip_addr, new_subnet, old_subnet);

                subnet_block_print(subnet_block);
                subnet_block_print_address_ranges(subnet_block);

                subnet_block_destructor(subnet_block);
                free(subnet_block);

                break;
        }
    } else {
        // Case 6:
        // subnet [Network address] ([0-9]*s:[0-9]*(i|h))+
        // Case 7:
        // subnet [Network address]/[Old CIDR mask] ([0-9]*s:[0-9]*(i|h))+
    }
    return 0;
}

/*

Input possibilities:
    Case 1: - Implemented
    subnet [Network address]/[New CIDR mask]

        Notes:
            The old subnet mask is a classful mask determined from the first octet of the [Network address].
            The new subnet mask is in CIDR format.

    Case 2: - Implemented
    subnet [Network address] [Dotted decimal subnet mask]

        Notes:
            The old subnet mask is a classful mask determined from the first octet of the [Network address].
            The new subnet mask is in dotted decimal notation.

    Case 3: - Implemented
    subnet [Network address]/[Old CIDR mask] [New dotted decimal subnet mask]

        Notes:
            The old subnet mask is in CIDR format. The new subnet mask is in dotted decimal notation.

    Case 4:
    subnet [Network address] [0-9]+(s|h)

        Notes:
            Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
            appropriate new subnet mask.  The old subnet mask is a classful mask determined from the first octet 
            of the [Network address].

    Case 5:
    subnet [Network address]/[Old CIDR mask] [0-9]+(s|h)

        Notes:
            Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
            appropriate new subnet mask.  The old subnet mask is in CIDR notation.

    Case 6:
    subnet [Network address] ([0-9]*s:[0-9]*(i|h))+


    Case 7:
    subnet [Network address]/[Old CIDR mask] ([0-9]*s:[0-9]*(i|h))+


exit codes
1 - exited in the main from wrong number of arguments
2 - exited from invalid IP block
3 - exited from invalid subnet mask

I like this better now with the error checking outside of the 
*/