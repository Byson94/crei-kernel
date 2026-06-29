#pragma once

#define PANIC(msg) kernel_panic(msg, __FILE__, __LINE__)
__attribute__((noreturn)) void kernel_panic(const char *msg, const char *file, unsigned int line);

