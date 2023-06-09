/*
 ============================================================================
 Name        : hev-socks5.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2021 - 2023 hev
 Description : Socks5
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../../../third-part/hev-task-system/include/hev-task.h"
#include "../../../third-part/hev-task-system/include/hev-task-io.h"
#include "../../../third-part/hev-task-system/include/hev-task-io-socket.h"
#include "../../../third-part/hev-task-system/include/hev-task-dns.h"
#include "../../../third-part/hev-task-system/include/hev-memory-allocator.h"

#include "hev-socks5-logger-priv.h"

#include "hev-socks5.h"

int
hev_socks5_get_timeout (HevSocks5 *self)
{
    return self->timeout;
}

void
hev_socks5_set_timeout (HevSocks5 *self, int timeout)
{
    self->timeout = timeout;
}

void
hev_socks5_set_auth_user_pass (HevSocks5 *self, const char *user,
                               const char *pass)
{
    self->auth.user = user;
    self->auth.pass = pass;
}

static int
hev_socks5_bind (HevSocks5 *self, int sock)
{
    return 0;
}

int
hev_socks5_construct (HevSocks5 *self, HevSocks5Type type)
{
    int res;

    res = hev_object_construct (&self->base);
    if (res < 0)
        return res;

    LOG_D ("%p socks5 construct", self);

    HEV_OBJECT (self)->klass = HEV_SOCKS5_TYPE;

    self->fd = -1;
    self->timeout = -1;
    self->type = type;

    return 0;
}

static void
hev_socks5_destruct (HevObject *base)
{
    HevSocks5 *self = HEV_SOCKS5 (base);

    LOG_D ("%p socks5 destruct", self);

    if (self->fd >= 0)
        close (self->fd);

    HEV_OBJECT_TYPE->finalizer (base);
    hev_free (base);
}

HevObjectClass *
hev_socks5_class (void)
{
    static HevSocks5Class klass;
    HevSocks5Class *kptr = &klass;
    HevObjectClass *okptr = HEV_OBJECT_CLASS (kptr);

    if (!okptr->name) {
        memcpy (kptr, HEV_OBJECT_TYPE, sizeof (HevObjectClass));

        okptr->name = "HevSocks5";
        okptr->finalizer = hev_socks5_destruct;

        kptr->binder = hev_socks5_bind;
    }

    return okptr;
}
