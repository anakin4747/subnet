#ifndef SUBNET_OPTIONS_H
#define SUBNET_OPTIONS_H

// 1 arg input of subnet block
void subnet_classless_cidr_mask(u_int32_t, int);

// 2 arg input of subnet block, IP address and D.D. mask
void subnet_classless_dd_mask(char*, char*);

// 2 arg input of subnet block and max expression
void subnet_max(char*, char*);

// Multiple arg input of subnet block and several subnet expressions
void subnet_expressions(char**);

#endif // SUBNET_OPTIONS_H