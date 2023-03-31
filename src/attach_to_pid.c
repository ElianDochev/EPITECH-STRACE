/*
** EPITECH PROJECT, 2023
** strace
** File description:
** attach_to_pid
*/

#include "syscall.h"
#include <string.h>

void attach_to_pid(short mask, int pid)
{
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        ERROR("ptrace: %s\n", strerror(errno));
        exit(EXIT_EPI_FAIL);
    }
    waitpid(pid, NULL, 0);
    parce_syscall(mask, pid);
}

void run_command(short mask, char **av)
{
    int pid = 0;

    pid = fork();
    if (pid == 0) {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            ERROR("ptrace: %s\n", strerror(errno));
            exit(EXIT_EPI_FAIL);
        }
        execvp(av[0], av);
        ERROR("execvp: %s\n", strerror(errno));
        exit(EXIT_EPI_FAIL);
    } else {
        waitpid(pid, NULL, 0);
        parce_syscall(mask, pid);
    }
}

void fill_args(regs_t *regs, arg_array_t *args)
{
    args->array[0] = regs->rdi;
    args->array[1] = regs->rsi;
    args->array[2] = regs->rdx;
    args->array[3] = regs->rcx;
    args->array[4] = regs->r8;
    args->array[5] = regs->r9;
}
