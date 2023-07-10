#include <stdlib.h>

u_int32_t dotted_decimal_to_32bit(u_int8_t first, u_int8_t second, u_int8_t third, u_int8_t fourth){
    return (first << 24) | (second << 16) | (third << 8) | (fourth);
}

u_int32_t cidr_to_32bit(int cidr){
    return (u_int32_t)(~0) << (32 - cidr);
}

int convert_32_mask_to_cdir(u_int32_t mask_32){
    int num_of_zeros;
    for(num_of_zeros = 0; (mask_32 & 1) == 0; num_of_zeros++, mask_32 >>= 1){ }
    return 32 - num_of_zeros;
}