#include "rpi.h"

// Implementation taken from newlib
// See: newlib/libc/string/strncmp.c
int strncmp(const char* s1, const char* s2, size_t n) {

    // Edge case: We don't want to do any dereferencing if the buffers aren't
    // valid
    if (n == 0)
        return 0;

    while (n-- != 0 && *s1 == *s2) {
        // Don't increment the pointers past the end of the array
        // Don't increment the pointers past the null-terminator either
        if (n == 0 || *s1 == '\0')
            break;
        s1++;
        s2++;
    }

    return (*(unsigned char *) s1) - (*(unsigned char *) s2);
}
