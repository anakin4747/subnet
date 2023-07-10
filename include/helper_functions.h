#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H


u_int32_t dotted_decimal_to_32bit(u_int8_t first, u_int8_t second, u_int8_t third, u_int8_t fourth);

u_int32_t cidr_to_32bit(int cidr);

int convert_32_mask_to_cdir(u_int32_t mask_32);

#endif // HELPER_FUNCTIONS_H