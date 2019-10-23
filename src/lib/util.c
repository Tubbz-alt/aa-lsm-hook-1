/*
 * This file is part of aa-lsm-hook.
 *
 * Copyright © 2018-2019 Solus Project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define _GNU_SOURCE

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "util.h"

/**
 * Taken directly from our usysconf codebase
 */
int aa_lsm_hook_exec_command(char **command)
{
        pid_t p;
        int status = 0;
        int ret = 0;
        int r = -1;

        if ((p = fork()) < 0) {
                fprintf(stderr, "Failed to fork(): %s\n", strerror(errno));
                return -1;
        } else if (p == 0) {
                /* Execute the command */
                if ((r = execv(command[0], command)) != 0) {
                        fprintf(stderr, "Failed to execve(%s): %s\n", command[0], strerror(errno));
                        exit(EXIT_FAILURE);
                }
                _exit(EXIT_SUCCESS);
                /* We'd never actually get here. */
        } else {
                if (waitpid(p, &status, 0) < 0) {
                        fprintf(stderr, "Failed to waitpid(%d): %s\n", (int)p, strerror(errno));
                        return -1;
                }
                /* i.e. sigsev */
                if (!WIFEXITED(status)) {
                        fprintf(stderr, "Child process '%s' exited abnormally\n", command[0]);
                }
        }

        /* At this point just make sure the return code was 0 */
        ret = WEXITSTATUS(status);
        return ret;
}

/*
 * Editor modelines  -  https://www.wireshark.org/tools/modelines.html
 *
 * Local variables:
 * c-basic-offset: 8
 * tab-width: 8
 * indent-tabs-mode: nil
 * End:
 *
 * vi: set shiftwidth=8 tabstop=8 expandtab:
 * :indentSize=8:tabSize=8:noTabs=true:
 */