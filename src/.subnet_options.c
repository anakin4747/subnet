// #include <stdio.h>
// #include <stdlib.h>
// #include <regex.h>

// #include "subnet_block.h"

// #define IP_ADDR_W_CIDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/([0-9]|[12][0-9]|3[0-2])$"

// typedef int bool_t;


// static bool_t __invalid_ip_addr_w_cidr(char* ip_addr){
//     // Save space on the stack I guess
//     regex_t* regex = (regex_t*)malloc(sizeof(regex_t));

//     // Compile regex
//     if(regcomp(regex, IP_ADDR_W_CIDR_REGEX, REG_EXTENDED)){
//         fprintf(stderr, "Could not compile REGEX\n");
//         free(regex);
//         return 1;
//     }

//     // Perform regex comparison
//     if(regexec(regex, ip_addr_w_cidr, 0, NULL, 0)){
//         free(regex);
//         return 1;
//     }

//     free(regex);
//     return 0;
// }

// static bool_t __is_classful(){

// }

// static bool_t __is_network_addr(){

// }

// // 1 arg input of subnet block
// void subnet_classless_cidr_mask(char* ip_addr_w_cidr){

// }

// // 2 arg input of subnet block, IP address and D.D. mask
// void subnet_classless_dd_mask(char*, char*){

// }

// // 2 arg input of subnet block and max expression
// void subnet_max(char*, char*){

// }

// // Multiple arg input of subnet block and several subnet expressions
// void subnet_expressions(char**){

// }