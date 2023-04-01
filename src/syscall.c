/*
** EPITECH PROJECT, 2023
** strace
** File description:
** syscall
*/

#include "syscall.h"

static int defer(syscall_t *syscall, int index)
{
    if (index == 0)
        return syscall->arg_one;
    if (index == 1)
        return syscall->arg_two;
    if (index == 2)
        return syscall->arg_three;
    if (index == 3)
        return syscall->arg_four;
    if (index == 4)
        return syscall->arg_five;
    if (index == 5)
        return syscall->arg_six;
}

static void print_value(intptr_t value, int type)
{
    char str[4096] = {0};

    if (type == STRING) {
        printf("\"");
        print_string(value, str);
        printf("\"");
        return;
    }
    if (type == NUM)
        printf("%ld", value);
    else
        printf("0x%lx", value);
}

void print_rags(arg_array_t *args, syscall_t *syscall, int index, short mask)
{
    if (!CHK_FLAG(mask, DISP_DETAILS)) {
        printf("0x%lx%s", args->array[index],
        index == syscall->num_args - 1 ? ") " : ", ");
        return;
    }
    print_value(args->array[index], defer(syscall, index));
    printf("%s", index == syscall->num_args - 1 ? ") " : ", ");
}

void print_ret(syscall_t *syscall, intptr_t value, short mask)
{
    if (!CHK_FLAG(mask, DISP_DETAILS)) {
        if (syscall->ret_type == VOID)
            printf("= ?\n");
        else
            printf("= 0x%lx\n", value);
        return;
    }
    if (syscall->ret_type == VOID)
        printf("= ?\n");
    else {
        printf("= ");
        print_value(value, syscall->ret_type);
        printf("\n");
    }
}

int do_syscall(regs_t *regs, short mask, int pid, int status)
{
    syscall_t syscall = regs->orig_rax < (sizeof(table) / sizeof(table[0])) ?
    table[regs->orig_rax] : table[299];
    arg_array_t args;

    if (syscall.code == -1)
        return 0;
    fill_args(regs, &args);
    printf("%s(", syscall.name);
    for (int i = 0; i < syscall.num_args; i++)
        print_rags(&args, &syscall, i, mask);
    if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1) {
        return perror("ptrace singlestep failed"), -1;
    }
    waitpid(pid, &status, 0);
    if (error_check(status, pid, regs) == -1)
        return -1;
    print_ret(&syscall, regs->rax, mask);
    return 0;
}
