/*
** EPITECH PROJECT, 2023
** strace
** File description:
** parce_syscall
*/

#include "syscall.h"

global_t *get_global(int running, short mask, int pid)
{
    static global_t global;

    if (running >= 0)
        global.running = running;
    if (mask >= 0)
        global.mask = mask;
    if (pid > 0)
        global.pid = pid;
    return &global;
}

int error_check(int status, int pid, regs_t *regs)
{
    if (WIFEXITED(status)) {
        printf("process exited\n");
        return -1;
    }
    if (!WIFSTOPPED(status) || WSTOPSIG(status) != SIGTRAP) {
        printf("unexpected stop signal\n");
        return -1;
    }
    if (ptrace(PTRACE_GETREGS, pid, NULL, regs) == -1) {
        perror("ptrace getregs failed");
        return -1;
    }
    if (!get_global(-1, -1, -1)->running)
        return -1;
    return 0;
}

static void sig_handler(int signo)
{
    global_t *global = get_global(-1, -1, -1);

    (void) signo;
    if (!CHK_FLAG(global->mask, PID_ATTACH)) {
        kill(global->pid, SIGTERM);
    }
    get_global(0, -1, -1);
}

void parce_syscall(short mask, int pid)
{
    regs_t regs;
    long data;
    int status;

    get_global(1, mask, pid);
    signal(SIGINT, sig_handler);
    while (get_global(-1, -1, -1)->running){
        if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1) {
            perror("ptrace singlestep failed");
            break;
        }
        waitpid(pid, &status, 0);
        if (error_check(status, pid, &regs) == -1)
            break;
        if (regs.orig_rax == -1)
            continue;
        if (do_syscall(&regs, mask, pid, status) == -1)
            break;
    }
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
}
