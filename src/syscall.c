/*
** EPITECH PROJECT, 2023
** strace
** File description:
** syscall
*/

#include "syscall.h"

int do_syscall(regs_t *regs, short mask, int pid, int status)
{
    syscall_t syscall = regs->orig_rax < (sizeof(table) / sizeof(table[0])) ?
    table[regs->orig_rax] : table[299];
    arg_array_t args;

    if (syscall.code == -1)
        return 0;
    fill_args(regs, &args);
    printf("\n%d\n", regs->orig_rax);
    printf("%s(", syscall.name);
    for (int i = 0; i < syscall.num_args; i++)
        printf("0x%lx%s", args.array[i], i == syscall.num_args - 1 ? ") " :
        ", ");
    if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1) {
        return perror("ptrace singlestep failed"), -1;
    }
    waitpid(pid, &status, 0);
    if (error_check(status, pid, regs) == -1)
        return -1;
    syscall.ret_type == VOID ? printf("= ?") : printf("= 0x%lx\n", regs->rax);
    return 0;
}
