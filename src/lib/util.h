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

#pragma once

#define __aa_unused__ __attribute__((unused))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define aa_unlikely(x) __builtin_expect((!!x), 0)

/**
 * Taken out of libnica and various other Solus projects like usysconf
 */
#define DEF_AUTOFREE(N, C)                                                                         \
        static inline void _autofree_func_##N(void *p)                                             \
        {                                                                                          \
                if (p && *(N **)p) {                                                               \
                        C(*(N **)p);                                                               \
                        (*(void **)p) = NULL;                                                      \
                }                                                                                  \
        }

#define autofree(N) __attribute__((cleanup(_autofree_func_##N))) N

/**
 * Execute a command and return the exit code if possible.
 * Non-zero exit status is considered an error.
 */
int aa_lsm_hook_exec_command(char **command);

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