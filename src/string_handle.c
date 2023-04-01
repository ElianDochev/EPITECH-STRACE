/*
** EPITECH PROJECT, 2023
** strace
** File description:
** string handle
*/

#include "syscall.h"

static void do_string(intptr_t value, char *str)
{
    long data;

    for (int i = 0; i < 4096; i += sizeof(long)) {
        data = ptrace(PTRACE_PEEKDATA, get_global(-1, -1, -1)->pid,
        value + i, NULL);
        if (data == -1)
            return perror("ptrace peekdata failed"), exit(84);
        memcpy(str + i, &data, sizeof(long));
        if (memchr(&data, 0, sizeof(long)) != NULL)
            break;
    }
}

void print_string(intptr_t value, char *str)
{
    do_string(value, str);
    for (int i = 0; i < 4096; i++) {
        if (str[i] == 0) {
            printf("\\0");
            break;
        }
        if (str[i] == '\n') {
            printf("\\n");
            continue;
        }
        if (str[i] == '\t') {
            printf("\\t");
            continue;
        }
        if (str[i] == '\r') {
            printf("\\r");
            continue;
        }
        printf("%c", str[i]);
    }
}
