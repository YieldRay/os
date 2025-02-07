#pragma once
#include "common.h"

__attribute__((noreturn)) void exit(void);
void putchar(char ch);

extern char _binary_shell_bin_start[];
extern char _binary_shell_bin_size[];