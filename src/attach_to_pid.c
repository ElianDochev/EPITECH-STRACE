/*
** EPITECH PROJECT, 2023
** strace
** File description:
** attach_to_pid
*/

#include "syscall.h"

void attach_to_pid(short mask, int pid)
{
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1) {
        ERROR("ptrace: %s\n", strerror(errno));
        exit(EXIT_EPI_FAIL);
    }
    waitpid(pid, NULL, 0);
    // will continue the process
}

void run_command(short mask, char **av)
{
    int pid = 0;

    if (!(pid = fork()))
    {
        if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
            ERROR("ptrace: %s\n", strerror(errno));
            exit(EXIT_EPI_FAIL);
        }
        execvp(av[0], av);
        ERROR("execvp: %s\n", strerror(errno));
        exit(EXIT_EPI_FAIL);
    } else {
        attach_to_pid(mask, pid);
    }
}
