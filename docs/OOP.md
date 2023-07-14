# Object Orientation in C

I have been studying how to implement object oriented concepts in C and in this project I was able to implement objects through creating classes and having composition relationships between objects.

I made 2 classes in this project, one for IP addresses and another for a block of subnets. The blocks of subnets are composed of several IP address objects themselves which makes a composition relationship.

In C, since there are no features in the language to support creating classes, it is on the programmer to maintain the implicit logical encapsulation of a class.


The IP address class header file is the API which it is seen as by the rest of the program. That means that the struct is only declared and not defined which makes the attributes of the ip_addr_t struct private. Note that each function for the ip_addr class accepts an ip_addr_t pointer. This is comparable to the self or this keyword in other languages. To help keep track of the components of the class, all the functions begin with the prefix ip_addr_.

    // src/ip_addr.h

    #ifndef IP_ADDR_H
    #define IP_ADDR_H

    // Forward declaration
    struct ip_addr_t;

    // Memory allocator
    struct ip_addr_t* ip_addr_malloc(void);

    // Constructor
    void ip_addr_constructor(struct ip_addr_t*, u_int32_t);

    // Destructor
    void ip_addr_destructor(struct ip_addr_t*);

    // Behaviour functions
    void ip_addr_print_dotted_decimal(struct ip_addr_t*);
    void ip_addr_print_binary(struct ip_addr_t*);
    u_int8_t ip_addr_get_octet(struct ip_addr_t*, int);
    u_int32_t ip_addr_get_32bit_value(struct ip_addr_t*);

    #endif // IP_ADDR_H

The subnet block class consumes this public API of the IP address class. This class contains 4 ip_addr_t pointers. These must be allocated, constructed, destructed, and freed by subnet_block_t class. The attributes of those ip_addr cannot be accessed by the API and must use the provided behaviour functions to interact with their attributes. The ip_addr pointers must be pointer as they have not been defined yet but a pointers size is always known. So you couldn't have a ip_addr variable in the subnet_block struct without making the ip_addr's attributes public. The pointer allows for the isolation and information hiding.

    // subnet_block.c
    
    #include "ip_addr.h"
    ...

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
    ...

I learnt this practice from the [Extreme C textbook](https://www.packtpub.com/free-ebook/extreme-c/9781789343625) by Kamran Amini.