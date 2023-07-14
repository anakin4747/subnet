#include <stdlib.h>
#include <stdio.h>

u_int32_t dotted_decimal_to_32bit(u_int8_t first, u_int8_t second, u_int8_t third, u_int8_t fourth){
    return (first << 24) | (second << 16) | (third << 8) | (fourth);
}

u_int32_t cidr_to_32bit(int cidr){
    return (u_int32_t)(~0) << (32 - cidr);
}

int convert_32_mask_to_cdir(u_int32_t mask_32){
    // I want to replace this with some digital logic instead so its quicker
    int num_of_zeros, i;
    for(num_of_zeros = i = 0; (mask_32 & 1) == 0 && i < 32; num_of_zeros++, mask_32 >>= 1, i++){ }
    return 32 - num_of_zeros;
}

#define COLOR_BOLD  "\e[1m"
#define COLOR_OFF   "\e[m"

void print_help_menu(void){
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
           "      subnet 10.0.0.0/9 255.192.0.0              # Old subnet mask is 9, new is 10\n"
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
           "      subnet 192.168.0.0 60s      # Calculate a new subnet mask to accommodate at least 60 subnets with the maximum\n"
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
           "      subnet 192.168.0.0/25 60s   # Calculate a new subnet mask to accommodate at least 60 subnets with the maximum\n"
           "                                  # hosts per subnet possible. The old subnet is obtained from the CIDR notation.\n"
           "\n"
           "      subnet 10.0.0.0/9 100h      # Calculate a new subnet mask to accommodate at least 100 hosts with the maximum\n"
           "                                  # number of subnets possible. The old subnet is obtained from the CIDR notation.\n\n");
}