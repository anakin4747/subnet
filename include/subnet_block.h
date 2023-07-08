#ifndef SUBNET_BLOCK_H
#define SUBNET_BLOCK_H

// Forward declaration
struct subnet_block_t;

// Memory allocator
struct subnet_block_t* subnet_block_malloc(void);

// Constructor
void subnet_block_constructor(struct subnet_block_t*, char*);

// Destructor
void subnet_block_destructor(struct subnet_block_t*);

// Behaviour functions
void subnet_block_print(struct subnet_block_t*);
void subnet_block_print_address_ranges(subnet_block_t*);

#endif // SUBNET_BLOCK_H