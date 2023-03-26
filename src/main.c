/*
** EPITECH PROJECT, 2023
** strace
** File description:
** main
*/

#include "syscall.h"

extern char *optarg;
extern int optind;

static int my_atoi(const char* str)
{
    int result = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        ++str;
    }
    while (*str) {
        if (!isdigit(*str)) {
            ERROR("Invalid char in string: %c\n", *str);
            errno = EINVAL;
            return -1;
        }
        result = result * 10 + (*str - '0');
        ++str;
    }
    return result * sign;
}

static int parce_args(short *mask,int ac, char **av)
{
    int pid = 0;

     for (int opt = 0; (opt = getopt(ac, av, "p:s")) != -1;) {
        switch (opt) {
            case 'p':
                SET_FLAG(*mask, PID_ATTACH);
                pid = my_atoi(optarg);
                break;
            case 's':
                SET_FLAG(*mask, DISP_DETAILS);
                break;
            default:
                ERROR("Usage: %s [-s] [-p <pid>] <command> [args]\n", av[0]);
                exit(EXIT_EPI_FAIL);
        }
    }
    return pid;
}

int main(int ac, char **av)
{
    short mask = 0;
    int pid = 0;

    if (ac < 2) {
        ERROR("Usage: %s [-s] [-p <pid>] <command> [args]\n", av[0]);
        return EXIT_EPI_FAIL;
    }
    if ((pid = parce_args(&mask, ac, av)) < 0 && CHK_FLAG(mask, PID_ATTACH))
        return EXIT_EPI_FAIL;
    CHK_FLAG(mask, PID_ATTACH) ? attach_to_pid(mask, pid) : run_command(mask, av + optind);
    return 0;
}
