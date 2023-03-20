/*
 ============================================================================
 Name        : hev-main.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2017 - 2023 hev
 Description : Main
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>

#include "../third-part/hev-task-system/include/hev-task.h"
#include "core/include/hev-socks5-misc.h"
#include "core/include/hev-socks5-logger.h"

#include "hev-config.h"
#include "hev-config-const.h"
#include "misc/hev-logger.h"
#include "hev-socks5-proxy.h"

#include "hev-main.h"

#define COMMIT_ID "<not set>"

static void
show_help (const char *self_path)
{
    printf ("%s CONFIG_PATH\n", self_path);
    printf ("Version: %u.%u.%u %s\n", MAJOR_VERSION, MINOR_VERSION,
            MICRO_VERSION, COMMIT_ID);
}

static void
run_as_daemon (const char *pid_file)
{
    FILE *fp;

    fp = fopen (pid_file, "w+");
    if (!fp) {
        LOG_E ("open pid file %s", pid_file);
        return;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    if (daemon (0, 0)) {
        /* ignore return value */
    }
#pragma GCC diagnostic pop

    fprintf (fp, "%u\n", getpid ());
    fclose (fp);
}

static int
set_limit_nofile (int limit_nofile)
{
    struct rlimit limit = {
        .rlim_cur = limit_nofile,
        .rlim_max = limit_nofile,
    };

    return setrlimit (RLIMIT_NOFILE, &limit);
}

int
main (int argc, char *argv[])
{
    const char *pid_file;
    const char *log_file;
    int log_level;
    int nofile;
    int res;

    if (argc != 2) {
        show_help (argv[0]);
        return -1;
    }

    res = hev_config_init (argv[1]);
    if (res < 0)
        return -2;

    log_file = hev_config_get_misc_log_file ();
    log_level = hev_config_get_misc_log_level ();

    res = hev_config_get_misc_task_stack_size ();
    hev_socks5_set_task_stack_size (res);

    res = hev_config_get_misc_udp_recv_buffer_size ();
    hev_socks5_set_udp_recv_buffer_size (res);

    res = hev_logger_init (log_level, log_file);
    if (res < 0)
        return -3;

    res = hev_socks5_logger_init (log_level, log_file);
    if (res < 0)
        return -4;

    res = hev_socks5_proxy_init ();
    if (res < 0)
        return -5;

    nofile = hev_config_get_misc_limit_nofile ();
    res = set_limit_nofile (nofile);
    if (res < 0)
        LOG_W ("set limit nofile");

    pid_file = hev_config_get_misc_pid_file ();
    if (pid_file)
        run_as_daemon (pid_file);

    hev_socks5_proxy_run ();

    hev_socks5_proxy_fini ();
    hev_socks5_logger_fini ();
    hev_logger_fini ();
    hev_config_fini ();

    return 0;
}
