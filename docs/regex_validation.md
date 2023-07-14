# Regex Validation

Regular expressions are used in src/input.c for filtering the input for correct syntax. The following regex was used to identify valid strings containing IP addresses.

    #define IP_ADDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"

The following regex was used to identify valid IP address with a CIDR mask (slash notation).

    #define IP_ADDR_W_CIDR_REGEX "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)/([0-9]|[12][0-9]|3[0-2])$"

The following regex was used for cases 4 and 5 to match a max expression. A max expression in this application is any number or no number followed by an h or an s.

    #define MAX_EXPRESSION_REGEX "^[0-9]{0,10}(h|s)$"

The __invalid_ip_addr function is one of the functions which performs the regex comparison. It is static and marked with a dunder prefix because it is private to the file src/input.c. To maintain a standard for validation functions in this project, they all test if invalid so that a successful exit code is 0.

    // Regex validation function
    static bool_t __invalid_ip_addr(char* ip_addr, const char* ip_exp){
        regex_t* regex = (regex_t*)malloc(sizeof(regex_t));

        // Compile regex
        if(regcomp(regex, ip_exp, REG_EXTENDED)){
            fprintf(stderr, "Could not compile REGEX\n");
            free(regex);
            return 1;
        }

        // Perform regex comparison
        if(regexec(regex, ip_addr, 0, NULL, 0)){
            free(regex);
            return 1;
        }

        free(regex);
        return 0;
    }