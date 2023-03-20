/*
 ============================================================================
 Name        : hev-socks5-session.h
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2017 - 2021 hev
 Description : Socks5 Session
 ============================================================================
 */

#ifndef __HEV_SOCKS5_SESSION_H__
#define __HEV_SOCKS5_SESSION_H__

#include "../third-part/hev-task-system/include/hev-task.h"
#include "core/include/hev-socks5-server.h"

#include "misc/hev-list.h"

#define HEV_SOCKS5_SESSION(p) ((HevSocks5Session *)p)
#define HEV_SOCKS5_SESSION_CLASS(p) ((HevSocks5SessionClass *)p)
#define HEV_SOCKS5_SESSION_TYPE (hev_socks5_session_class ())

typedef struct _HevSocks5Session HevSocks5Session;
typedef struct _HevSocks5SessionClass HevSocks5SessionClass;

struct _HevSocks5Session
{
    HevSocks5Server base;

    HevListNode node;
    HevTask *task;
    void *data;
};

struct _HevSocks5SessionClass
{
    HevSocks5ServerClass base;
};

HevObjectClass *hev_socks5_session_class (void);

int hev_socks5_session_construct (HevSocks5Session *self, int fd);

HevSocks5Session *hev_socks5_session_new (int fd);

void hev_socks5_session_terminate (HevSocks5Session *self);

#endif /* __HEV_SOCKS5_SESSION_H__ */
