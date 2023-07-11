#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ip_addr.h"
#include "helper_functions.h"

// Subnet block class
typedef struct {
    int num_of_subnets;
    int hosts_per_subnet;
    int new_mask_cidr;
    int old_mask_cidr;
    char class;

    struct ip_addr_t* network_addr;
    struct ip_addr_t* broadcast_addr;
    struct ip_addr_t* new_subnet_mask;
    struct ip_addr_t* old_subnet_mask;
} subnet_block_t;

// Memory allocator
subnet_block_t* subnet_block_malloc(void){
    return (subnet_block_t*)malloc(sizeof(subnet_block_t));
}

static int __calculate_num_of_subnets(int mask_cidr, int original_mask_cidr){
    return (int)pow(2, (mask_cidr - original_mask_cidr));
}

static int __calculate_hosts_per_subnet(int mask_cidr){
    return (int)(pow(2, (32 - mask_cidr)) - 2);
}

static char __class_from_first_octet(struct ip_addr_t* ip_addr){
    u_int8_t first_octet = ip_addr_get_octet(ip_addr, 1);

    if(first_octet <= 126){
        return 'A';
    } else if(first_octet >= 128 && first_octet <= 191){
        return 'B';
    } else if(first_octet >= 192 && first_octet <= 223){
        return 'C';
    } else {
        return '\0';
    }
}

// Constructor 
void subnet_block_constructor(subnet_block_t* subnet_block,
                              u_int32_t net_addr_32,
                              u_int32_t new_mask_32,
                              u_int32_t old_mask_32){

    // Allocating and constructing IP addresses for Network, Broadcast, new Subnet, and old Subnet
    subnet_block->network_addr = ip_addr_malloc();
    subnet_block->broadcast_addr = ip_addr_malloc();
    subnet_block->new_subnet_mask = ip_addr_malloc();
    subnet_block->old_subnet_mask = ip_addr_malloc();
    ip_addr_constructor(subnet_block->network_addr, net_addr_32);
    ip_addr_constructor(subnet_block->broadcast_addr, net_addr_32 | ~old_mask_32);
    ip_addr_constructor(subnet_block->new_subnet_mask, new_mask_32);
    ip_addr_constructor(subnet_block->old_subnet_mask, old_mask_32);

    // Save CIDR subnet mask
    subnet_block->new_mask_cidr = convert_32_mask_to_cdir(new_mask_32);
    subnet_block->old_mask_cidr = convert_32_mask_to_cdir(old_mask_32);

    // Calculate number of subnets
    subnet_block->num_of_subnets = __calculate_num_of_subnets(subnet_block->new_mask_cidr, subnet_block->old_mask_cidr);

    // Calculate number of hosts possible from the new CIDR mask
    subnet_block->hosts_per_subnet = __calculate_hosts_per_subnet(subnet_block->new_mask_cidr);

    // Save class of network address
    subnet_block->class = __class_from_first_octet(subnet_block->network_addr);
}

void subnet_block_destructor(subnet_block_t* subnet_block){

    // Deconstruct and free all allocated IP addresses
    ip_addr_destructor(subnet_block->network_addr);
    ip_addr_destructor(subnet_block->broadcast_addr);
    ip_addr_destructor(subnet_block->new_subnet_mask);
    ip_addr_destructor(subnet_block->old_subnet_mask);
    free(subnet_block->network_addr);
    free(subnet_block->broadcast_addr);
    free(subnet_block->new_subnet_mask);
    free(subnet_block->old_subnet_mask);
}

void subnet_block_print(subnet_block_t* subnet_block){

    printf("\nNumber of subnets possible: %d\n"
           "Number of hosts per subnet: %d\n"
           "New subnet mask in CIDR notation: %d\n"
           "Old subnet mask in CIDR notation: %d\n"
           "Class of subnet: %c\n",
           subnet_block->num_of_subnets,
           subnet_block->hosts_per_subnet,
           subnet_block->new_mask_cidr,
           subnet_block->old_mask_cidr,
           subnet_block->class);
    
    printf("Network address: ");
    ip_addr_print_dotted_decimal(subnet_block->network_addr);
    printf("\nBroadcast address: ");
    ip_addr_print_dotted_decimal(subnet_block->broadcast_addr);
    printf("\nNew subnet mask: ");
    ip_addr_print_dotted_decimal(subnet_block->new_subnet_mask);
    printf("\nOld subnet mask: ");
    ip_addr_print_dotted_decimal(subnet_block->old_subnet_mask);
    printf("\n");
}

void subnet_block_print_address_ranges(subnet_block_t* subnet_block){
    int interesting_octet = (subnet_block->new_mask_cidr + 8) / 8;
    printf("Interesting octet: %d\n", interesting_octet);

    int block_size = 256 - ip_addr_get_octet(subnet_block->new_subnet_mask, interesting_octet);
    printf("Block size: %d\n", block_size);

    // Instead of making these strings maybe just convert this to actually creating 4 ip_addr_t per subnet

    for(int i = 0; i < subnet_block->num_of_subnets; i++){
        struct ip_addr_t* net_ip = ip_addr_malloc();
        struct ip_addr_t* broad_ip = ip_addr_malloc();
        struct ip_addr_t* first_usable_ip = ip_addr_malloc();
        struct ip_addr_t* last_usable_ip = ip_addr_malloc();

        u_int32_t net_32 = ip_addr_get_32bit_value(subnet_block->network_addr);
        u_int32_t broad_32;

        // Clear interesting octet
        net_32 &= ~(0xFF << ((4 - interesting_octet) * 8));

        // Set interesting octet
        net_32 |= (u_int8_t)(block_size * i) << ((4 - interesting_octet) * 8);
        broad_32 = net_32 | ~ip_addr_get_32bit_value(subnet_block->new_subnet_mask);

        ip_addr_constructor(net_ip, net_32);
        ip_addr_constructor(broad_ip, broad_32);
        ip_addr_constructor(first_usable_ip, net_32 + 1);
        ip_addr_constructor(last_usable_ip, broad_32 - 1);


        printf("\nSubnet: ");
        ip_addr_print_dotted_decimal(net_ip);
        printf("/%d\n", subnet_block->new_mask_cidr);
        printf("  Directed Broadcast Address: ");
        ip_addr_print_dotted_decimal(broad_ip);
        printf("\n    Usable Address Range: ");
        ip_addr_print_dotted_decimal(first_usable_ip);
        printf(" - ");
        ip_addr_print_dotted_decimal(last_usable_ip);
        printf("\n");

        ip_addr_destructor(net_ip);
        ip_addr_destructor(broad_ip);
        ip_addr_destructor(first_usable_ip);
        ip_addr_destructor(last_usable_ip);

        free(net_ip);
        free(broad_ip);
        free(first_usable_ip);
        free(last_usable_ip);
    }
    printf("\n");
}

// Testing purposes
/*
int main(){
    // // printf("%d\n", convert_32_mask_to_cdir(0xFFFFFF00));

    subnet_block_t* sn_blk = subnet_block_malloc();
    subnet_block_constructor(sn_blk, 
                             dotted_decimal_to_32bit(192, 168, 0, 0),
                             dotted_decimal_to_32bit(255, 255, 255, 192),
                             dotted_decimal_to_32bit(255, 255, 255, 0));

    subnet_block_print(sn_blk);
    subnet_block_print_address_ranges(sn_blk);


    subnet_block_destructor(sn_blk);
    free(sn_blk);

    return 0;
}
*/