#ifndef INPUT_H
#define INPUT_H

/*
   Accepts first 2 argv for IP address and Mask,
    the second is NULL for case 1: subnet [ip]/[cidr]
   Three int pointers for returning the IP and both Masks
   Returns error codes:
    0 - Success
    1 - Nothing
    2 - Wrong IP format
    3 - Wrong mask format
                                                            */

int process_input_ip_and_masks(char* arg1, char* arg2, u_int32_t* ip_addr, u_int32_t* new_subnet, u_int32_t* old_subnet);

#endif // INPUT_H