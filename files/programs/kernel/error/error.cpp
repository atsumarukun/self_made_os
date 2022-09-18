#include <stdio.h>

#include "error.hpp"

char* Error::Message() {
    char error_message[1024];
    sprintf(error_message, "%s, file: %s, line: %d", error_message_, file_, line_);
    return error_message;
}