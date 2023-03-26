/*
** EPITECH PROJECT, 2023
** strace
** File description:
** syscall
*/

#include "syscall.h"

int do_syscall(regs_t *regs, short mask, int pid, int status)
{
    syscall_t syscall = table[regs->orig_rax];
    arg_array_t args;

    fill_args(regs, &args);
    printf("%s(", syscall.name);
    for (int i = 0; i < syscall.num_args; i++)
        printf("0x%lx%s", args.array[i], i == syscall.num_args - 1 ? ") " :
        ", ");
    if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1) {
        perror("ptrace singlestep failed");
        return -1;
    }
    waitpid(pid, &status, 0);
    if (error_check(status, pid, regs) == -1)
        return -1;
    syscall.ret_type == VOID ? printf("= ?") : printf("= 0x%lx\n", regs->rax);
    return 0;
}
