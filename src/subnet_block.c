#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ip_addr.h"

typedef int bool_t;

typedef struct {
    int num_of_subnets;
    int hosts_per_subnet;
    int new_mask_cidr;
    int old_mask_cidr;
    bool_t is_classful; // Maybe remove
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

int __calculate_num_of_subnets(int mask_cidr, int original_mask_cidr){
    return (int)pow(2, (mask_cidr - original_mask_cidr));
}

int __calculate_hosts_per_subnet(int mask_cidr){
    return (int)(pow(2, (32 - mask_cidr)) - 2);
}

int __convert_32_mask_to_cdir(u_int32_t mask_32){
    int num_of_zeros;
    for(num_of_zeros = 0; (mask_32 & 1) == 0; num_of_zeros++, mask_32 >>= 1){ }
    return 32 - num_of_zeros;
}

// Not actually fully correct since it doesn't check which class
// Maybe just remove and have error checking in the code which creates these objects
bool_t __subnet_block_is_classful(int mask_cidr){
    return mask_cidr % 8 == 0;
}

char __class_from_first_octet(struct ip_addr_t* ip_addr){
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
    ip_addr_constructor(subnet_block->broadcast_addr, net_addr_32 | ~new_mask_32);
    ip_addr_constructor(subnet_block->new_subnet_mask, new_mask_32);
    ip_addr_constructor(subnet_block->old_subnet_mask, old_mask_32);

    // Save CIDR subnet mask
    subnet_block->new_mask_cidr = __convert_32_mask_to_cdir(new_mask_32);
    subnet_block->old_mask_cidr = __convert_32_mask_to_cdir(old_mask_32);

    // Calculate number of subnets
    subnet_block->num_of_subnets = __calculate_num_of_subnets(subnet_block->new_mask_cidr, subnet_block->old_mask_cidr);

    // Calculate number of hosts possible from the new CIDR mask
    subnet_block->hosts_per_subnet = __calculate_hosts_per_subnet(subnet_block->new_mask_cidr);

    // Save class of network address
    subnet_block->class = __class_from_first_octet(subnet_block->network_addr);

    // Store if block is classful
    subnet_block->is_classful = __subnet_block_is_classful(subnet_block->new_mask_cidr);
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

    printf("Number of subnets possible: %d\n"
           "Number of hosts per subnet: %d\n"
           "New subnet mask in CIDR notation: %d\n"
           "Old subnet mask in CIDR notation: %d\n"
           "Is the subnet block classful: %s\n"
           "Class of subnet: %c\n",
           subnet_block->num_of_subnets,
           subnet_block->hosts_per_subnet,
           subnet_block->new_mask_cidr,
           subnet_block->old_mask_cidr,
           subnet_block->is_classful ? "True" : "False",
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

    char octet_buffer[12];

    for(int i = 1; i < interesting_octet; i++){
        printf("%d.", ip_addr_get_octet(subnet_block->network_addr, i));
    }

    printf("%d", ip_addr_get_octet(subnet_block->network_addr, interesting_octet) + block_size);
    printf("\n");
}

u_int32_t dotted_decimal_to_32bit(u_int8_t first, u_int8_t second, u_int8_t third, u_int8_t fourth){
    return (first << 24) | (second << 16) | (third << 8) | (fourth);
}

// Testing purposes
int main(){
    // printf("%d\n", __convert_32_mask_to_cdir(0xFFFFFF00));

    // for(int i = 0; i < 32; i++){
    //     printf("%d %d\n", i, __subnet_block_is_classful(i));
    // }

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