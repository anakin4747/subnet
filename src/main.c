#include <stdio.h>
#include <stdlib.h>

#include "subnet_block.h"
#include "input.h"
#include "helper_functions.h"

#define MAX_ARG_SIZE 4

int main(int argc, char** argv){

    u_int32_t ip_addr, new_subnet, old_subnet, result;

    if(argc == 1){
        // Help menu if no arguments are passed
        print_help_menu();
        exit(1);
    } else if(argc >= MAX_ARG_SIZE){
        printf("Too many arguments\n");
        exit(1);
    } else if(argc == 2){
        // Case 1:
        argv[2] = NULL;
    } 

    // This is common for cases 1-5

    // Gather the IP address and the new and old masks from the command line arguments
    result = process_input_ip_and_masks(argv[1], argv[2], &ip_addr, &new_subnet, &old_subnet);
    if(result){
        exit(result);
    }

    // Allocate a subnet_block object
    struct subnet_block_t* subnet_block = subnet_block_malloc();

    // Construct the object with the values collected from argv
    subnet_block_constructor(subnet_block, ip_addr, new_subnet, old_subnet);

    // Print subnet_block's attributes
    subnet_block_print(subnet_block);
    // Print the address ranges and other important addresses of the subnet_block object
    subnet_block_print_address_ranges(subnet_block);

    // Destroy and free object
    subnet_block_destructor(subnet_block);
    free(subnet_block);

    return 0;
}

/*

exit codes
1 - exited in the main from wrong number of arguments
2 - exited from invalid IP block
3 - exited from invalid subnet mask
4 - exited from invalid max expression

*/