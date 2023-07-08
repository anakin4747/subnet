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

#endif // IP_ADDR_H