/*
 ============================================================================
 Name        : hev-task-io-socket.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2018 everyone.
 Description : Task socket I/O operations
 ============================================================================
 */

#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "../../../kern/task/hev-task.h"
#include "../basic/hev-task-io.h"
#include "../../misc/hev-compiler.h"

#include "hev-task-io-socket.h"

EXPORT_SYMBOL int
hev_task_io_socket_socket (int domain, int type, int protocol)
{
    int fd, nonblock = 1;

    fd = socket (domain, type, protocol);
    if (0 > fd)
        return fd;

    if (0 > ioctl (fd, FIONBIO, (char *)&nonblock)) {
        close (fd);
        return -2;
    }

    return fd;
}

EXPORT_SYMBOL int
hev_task_io_socket_socketpair (int domain, int type, int protocol,
                               int socket_vector[2])
{
    int nonblock = 1;

    if (0 > socketpair (domain, type, protocol, socket_vector))
        return -1;

    if (0 > ioctl (socket_vector[0], FIONBIO, (char *)&nonblock)) {
        close (socket_vector[0]);
        close (socket_vector[1]);
        return -2;
    }

    if (0 > ioctl (socket_vector[1], FIONBIO, (char *)&nonblock)) {
        close (socket_vector[0]);
        close (socket_vector[1]);
        return -3;
    }

    return 0;
}

EXPORT_SYMBOL int
hev_task_io_socket_connect (int fd, const struct sockaddr *addr,
                            socklen_t addr_len, HevTaskIOYielder yielder,
                            void *yielder_data)
{
    int ret;
retry:
    ret = connect (fd, addr, addr_len);
    if (ret < 0) {
        if (errno == EINPROGRESS || errno == EALREADY) {
            if (yielder) {
                if (yielder (HEV_TASK_WAITIO, yielder_data))
                    return -2;
            } else {
                hev_task_yield (HEV_TASK_WAITIO);
            }
            goto retry;
        } else if (errno == EISCONN) {
            ret = 0;
        }
    }

    return ret;
}

EXPORT_SYMBOL int
hev_task_io_socket_accept (int fd, struct sockaddr *addr, socklen_t *addr_len,
                           HevTaskIOYielder yielder, void *yielder_data)
{
    int new_fd, nonblock = 1;

retry:
    new_fd = accept (fd, addr, addr_len);
    if (new_fd < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (new_fd >= 0) {
        if (ioctl (new_fd, FIONBIO, (char *)&nonblock) < 0) {
            close (new_fd);
            return -3;
        }
    }

    return new_fd;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_recv (int fd, void *buf, size_t len, int flags,
                         HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t size = 0;

retry:
    s = recv (fd, buf + size, len - size, flags & ~MSG_WAITALL);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len)
        goto retry;

    return size;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_send (int fd, const void *buf, size_t len, int flags,
                         HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t size = 0;

retry:
    s = send (fd, buf + size, len - size, flags & ~MSG_WAITALL);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len)
        goto retry;

    return size;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_recvfrom (int fd, void *buf, size_t len, int flags,
                             struct sockaddr *addr, socklen_t *addr_len,
                             HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t size = 0;

retry:
    s = recvfrom (fd, buf + size, len - size, flags & ~MSG_WAITALL, addr,
                  addr_len);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len)
        goto retry;

    return size;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_sendto (int fd, const void *buf, size_t len, int flags,
                           const struct sockaddr *addr, socklen_t addr_len,
                           HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t size = 0;

retry:
    s = sendto (fd, buf + size, len - size, flags & ~MSG_WAITALL, addr,
                addr_len);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len)
        goto retry;

    return size;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_recvmsg (int fd, struct msghdr *msg, int flags,
                            HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t i, size = 0, len = 0;
    struct msghdr mh;
    struct iovec iov[msg->msg_iovlen];

    mh.msg_name = msg->msg_name;
    mh.msg_namelen = msg->msg_namelen;
    mh.msg_control = msg->msg_control;
    mh.msg_controllen = msg->msg_controllen;
    mh.msg_flags = msg->msg_flags;
    mh.msg_iov = iov;
    mh.msg_iovlen = msg->msg_iovlen;

    for (i = 0; i < msg->msg_iovlen; i++) {
        iov[i] = msg->msg_iov[i];
        len += iov[i].iov_len;
    }

retry:
    s = recvmsg (fd, &mh, flags & ~MSG_WAITALL);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len) {
        for (i = 0; i < mh.msg_iovlen; i++) {
            if (s < mh.msg_iov[i].iov_len) {
                mh.msg_iov[i].iov_base += s;
                mh.msg_iov[i].iov_len -= s;
                break;
            }

            s -= mh.msg_iov[i].iov_len;
        }

        mh.msg_iov += i;
        mh.msg_iovlen -= i;

        goto retry;
    }

    return size;
}

EXPORT_SYMBOL ssize_t
hev_task_io_socket_sendmsg (int fd, const struct msghdr *msg, int flags,
                            HevTaskIOYielder yielder, void *yielder_data)
{
    ssize_t s;
    size_t i, size = 0, len = 0;
    struct msghdr mh;
    struct iovec iov[msg->msg_iovlen];

    mh.msg_name = msg->msg_name;
    mh.msg_namelen = msg->msg_namelen;
    mh.msg_control = msg->msg_control;
    mh.msg_controllen = msg->msg_controllen;
    mh.msg_flags = msg->msg_flags;
    mh.msg_iov = iov;
    mh.msg_iovlen = msg->msg_iovlen;

    for (i = 0; i < msg->msg_iovlen; i++) {
        iov[i] = msg->msg_iov[i];
        len += iov[i].iov_len;
    }

retry:
    s = sendmsg (fd, &mh, flags & ~MSG_WAITALL);
    if (s < 0 && errno == EAGAIN) {
        if (yielder) {
            if (yielder (HEV_TASK_WAITIO, yielder_data))
                return -2;
        } else {
            hev_task_yield (HEV_TASK_WAITIO);
        }
        goto retry;
    }

    if (!(flags & MSG_WAITALL))
        return s;

    if (s <= 0)
        return size;

    size += s;
    if (size < len) {
        for (i = 0; i < mh.msg_iovlen; i++) {
            if (s < mh.msg_iov[i].iov_len) {
                mh.msg_iov[i].iov_base += s;
                mh.msg_iov[i].iov_len -= s;
                break;
            }

            s -= mh.msg_iov[i].iov_len;
        }

        mh.msg_iov += i;
        mh.msg_iovlen -= i;

        goto retry;
    }

    return size;
}
