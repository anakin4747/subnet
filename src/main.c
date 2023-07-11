#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "subnet_block.h"
#include "input.h"

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

#define MAX_ARG_SIZE 4

int main(int argc, char** argv){

    u_int32_t ip_addr, new_subnet, old_subnet, result;

    if(argc == 1){
        printf("\nSubnet is a program which prints all the information of a subnet provided.\n"
               "\nPossible Inputs:\n"
               "-----------------------------------------------------------------------------------------------------------------------"
               "\n"
               COLOR_BOLD
               "  Case 1 (Classful & CIDR):\n"
               COLOR_OFF
               "    subnet [Network address]/[New CIDR mask]\n"
               "\n"
               "    Notes:\n"
               "      The old subnet mask is a classful mask determined from the first octet of the [Network address].\n"
               "      The new subnet mask is in CIDR format.\n"
               "\n"
               "    Examples:\n"
               "      subnet 192.168.0.0/25       # Old subnet mask is 24, new is 25\n"
               "      subnet 10.0.0.0/10          # Old subnet mask is 8, new is 10\n"
               "\n"
               "-----------------------------------------------------------------------------------------------------------------------"
               "\n"
               COLOR_BOLD
               "  Case 2 (Classful & Dotted Decimal):\n"
               COLOR_OFF
               "    subnet [Network address] [Dotted decimal subnet mask]\n"
               "\n"
               "    Notes:\n"
               "      The old subnet mask is a classful mask determined from the first octet of the [Network address].\n"
               "      The new subnet mask is in dotted decimal notation.\n"
               "\n"
               "    Examples:\n"
               "      subnet 192.168.0.0 255.255.255.128      # Old subnet mask is 24, new is 25\n"
               "      subnet 10.0.0.0 255.128.0.0             # Old subnet mask is 8, new is 9\n"
               "\n"
               "-----------------------------------------------------------------------------------------------------------------------"
               "\n"
               COLOR_BOLD
               "  Case 3 (CIDR & Dotted Decimal):\n"
               COLOR_OFF
               "    subnet [Network address]/[Old CIDR mask] [New dotted decimal subnet mask]\n"
               "\n"
               "    Notes:\n"
               "      The old subnet mask is in CIDR format. The new subnet mask is in dotted decimal notation.\n"
               "\n"
               "    Examples:\n"
               "      subnet 192.168.0.0/25 255.255.255.192      # Old subnet mask is 25, new is 26\n"
               "      subnet 10.0.0.0/9 255.192.0.0             # Old subnet mask is 9, new is 10\n"
               "\n"
               "-----------------------------------------------------------------------------------------------------------------------"
               "\n"
               COLOR_BOLD
               "  Case 4 (Classful Maximum Subnets|Hosts):\n"
               COLOR_OFF
               "    subnet [Network address] [0-9]{0,10}(s|h)\n"
               "\n"
               "    Notes:\n"
               "      Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an\n"
               "      appropriate new subnet mask.  The old subnet mask is a classful mask determined from the first octet\n"
               "      of the [Network address].\n"
               "\n"
               "    Examples:\n"
               "      subnet 192.168.0.0 100s     # Calculate a new subnet mask to accommodate at least 100 subnets with the maximum\n"
               "                                  # hosts per subnet possible. The old subnet is calculated from the class.\n"
               "\n"
               "      subnet 10.0.0.0 100h        # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum\n"
               "                                  # number of subnets possible. The old subnet is calculated from the class.\n"
               "\n"
               "-----------------------------------------------------------------------------------------------------------------------"
               "\n"
               COLOR_BOLD
               "  Case 5 (CIDR Maximum Subnets|Hosts):\n"
               COLOR_OFF
               "    subnet [Network address]/[Old CIDR mask] [0-9]{0,10}(s|h)\n"
               "\n"
               "    Notes:\n"
               "      Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an\n"
               "      appropriate new subnet mask.  The old subnet mask is in CIDR notation.\n"
               "\n"
               "    Examples:\n"
               "      subnet 192.168.0.0/25 100s  # Calculate a new subnet mask to accommodate at least 100 subnets with the maximum\n"
               "                                  # hosts per subnet possible. The old subnet is obtained from the CIDR notation.\n"
               "\n"
               "      subnet 10.0.0.0/9 100h      # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum\n"
               "                                  # number of subnets possible. The old subnet is obtained from the CIDR notation.\n\n");
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
        
    } else if(argc == 3){
        // Case 2 & 3:
        result = process_input_ip_and_mask(argv[1], argv[2], &ip_addr, &new_subnet, &old_subnet);
        if(result){
            exit(result);
        }
    } else {
        // Case 6:
        // subnet [Network address] ([0-9]*s:[0-9]*(i|h))+
        // Case 7:
        // subnet [Network address]/[Old CIDR mask] ([0-9]*s:[0-9]*(i|h))+
        return 0;
    }
    // This is common for cases 1-5
    struct subnet_block_t* subnet_block = subnet_block_malloc();

    subnet_block_constructor(subnet_block, ip_addr, new_subnet, old_subnet);

    subnet_block_print(subnet_block);
    subnet_block_print_address_ranges(subnet_block);

    subnet_block_destructor(subnet_block);
    free(subnet_block);

    return 0;
}

/*

Input possibilities:
    Case 1 (Classful & CIDR):
    subnet [Network address]/[New CIDR mask]

        Notes:
            The old subnet mask is a classful mask determined from the first octet of the [Network address].
            The new subnet mask is in CIDR format.
        
        Examples:
            subnet 192.168.0.0/25       # Old subnet mask is 24, new is 25
            subnet 10.0.0.0/10          # Old subnet mask is 8, new is 10

    Case 2 (Classful & Dotted Decimal): - Implemented
    subnet [Network address] [Dotted decimal subnet mask]

        Notes:
            The old subnet mask is a classful mask determined from the first octet of the [Network address].
            The new subnet mask is in dotted decimal notation.
        
        Examples:
            subnet 192.168.0.0 255.255.255.128      # Old subnet mask is 24, new is 25
            subnet 10.0.0.0 255.128.0.0             # Old subnet mask is 8, new is 9


    Case 3 (CIDR & Dotted Decimal):
    subnet [Network address]/[Old CIDR mask] [New dotted decimal subnet mask]

        Notes:
            The old subnet mask is in CIDR format. The new subnet mask is in dotted decimal notation.

        Examples:
            subnet 192.168.0.0/25 255.255.255.192      # Old subnet mask is 25, new is 26
            subnet 10.0.0.0/9 255.192.0.0             # Old subnet mask is 9, new is 10


    Case 4 (Classful Maximum Subnets|Hosts):
    subnet [Network address] [0-9]{0,10}(s|h)

        Notes:
            Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
            appropriate new subnet mask.  The old subnet mask is a classful mask determined from the first octet 
            of the [Network address].
        
        Examples:
            subnet 192.168.0.0 100s     # Calculate a new subnet mask to accommodate at least 100 subnets with the maximum 
                                        # hosts per subnet possible. The old subnet is calculated from the class.

            subnet 10.0.0.0 100h        # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum 
                                        # number of subnets possible. The old subnet is calculated from the class.

    Case 5 (CIDR Maximum Subnets|Hosts):
    subnet [Network address]/[Old CIDR mask] [0-9]+(s|h)

        Notes:
            Depending on the 's' or the 'h', this will maximize the number of subnets or hosts by choosing an 
            appropriate new subnet mask.  The old subnet mask is in CIDR notation.

        Examples:
            subnet 192.168.0.0/25 100s  # Calculate a new subnet mask to accommodate at least 100 subnets with the maximum 
                                        # hosts per subnet possible. The old subnet is obtained from the CIDR notation.

            subnet 10.0.0.0/9 100h      # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum 
                                        # number of subnets possible. The old subnet is obtained from the CIDR notation.

    Case 6: - Coming soon
    subnet [Network address] ([0-9]*s:[0-9]*(i|h))+


    Case 7: - Coming soon
    subnet [Network address]/[Old CIDR mask] ([0-9]*s:[0-9]*(i|h))+


exit codes
1 - exited in the main from wrong number of arguments
2 - exited from invalid IP block
3 - exited from invalid subnet mask
4 - exited from invalid max expression

*/