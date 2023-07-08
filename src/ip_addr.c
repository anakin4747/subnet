#include <stdio.h>
#include <stdlib.h>

// IP Address Class
typedef struct {
    union {
        u_int32_t ip_32_bits;
        u_int8_t octet_array[4];
    };
} ip_addr_t;
// I know its only a union in a struct which could just be a union
// But making the object attributes all structs provides some nice consistancy

// Memory allocator
ip_addr_t* ip_addr_malloc(void){
    return (ip_addr_t*)malloc(sizeof(ip_addr_t));
}

// Constructor
void ip_addr_constructor(ip_addr_t* ip_addr, u_int32_t ip_32_bits){
    ip_addr->ip_32_bits = ip_32_bits;
}

// Destructor
void ip_addr_destructor(ip_addr_t* ip_addr){
    // Nothing to do 
    // Keeping incase implementation requires destruction in the future
    ip_addr->ip_32_bits = 0; // Just doing this to suppress warning
}

// Behaviour functions
void ip_addr_print_dotted_decimal(ip_addr_t* ip_addr){
    printf("%d.%d.%d.%d",
           (ip_addr->ip_32_bits >> 24) & 0xFF,
           (ip_addr->ip_32_bits >> 16) & 0xFF,
           (ip_addr->ip_32_bits >> 8) & 0xFF,
           ip_addr->ip_32_bits & 0xFF);
}

void ip_addr_print_binary(ip_addr_t* ip_addr){
    for (int i = 31; i >= 0; i--) {
        u_int32_t mask = 1 << i;
        u_int32_t bit = (ip_addr->ip_32_bits & mask) ? 1 : 0;
        printf("%u", bit);
    }
}

u_int8_t ip_addr_get_octet(ip_addr_t* ip_addr, int octet){
    if(octet > 4 || octet < 1){
        fprintf(stderr, "Octet index out of bounds\n"
                        "In ip_addr_get_octet\n");
        exit(1);
    }
    return ip_addr->octet_array[4 - octet];
}

// Testing purposes
// int main(){
//     ip_addr_t* ip_addr = ip_addr_malloc();
//     // ip_addr_constructor(ip_addr, 3232235521);
//     ip_addr_constructor(ip_addr, (192 << 24) | (168 << 16) | (0 << 8) | 1);

//     printf("Dotted decimal notation: ");
//     ip_addr_print_dotted_decimal(ip_addr);
//     printf("Binary notation: ");
//     ip_addr_print_binary(ip_addr);
//     for(int i = 1; i < 5; i++){
//         printf("Octet %d: %d\n", i, ip_addr_get_octet(ip_addr, i));
//     }

//     ip_addr_destructor(ip_addr);
//     free(ip_addr);

//     return 0;
// }