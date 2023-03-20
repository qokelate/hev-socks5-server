/*
 ============================================================================
 Name        : hev-task-timer.c
 Author      : Heiher <r@hev.cc>
 Copyright   : Copyright (c) 2018 everyone.
 Description : Timer
 ============================================================================
 */

#include <time.h>

#include "hev-task-timer.h"

#if defined(__linux__)
#include "hev-task-timer-timerfd.h"
#else
#include "hev-task-timer-kevent.h"
#endif

#include "../../lib/misc/hev-compiler.h"

typedef struct _HevTaskTimerNode HevTaskTimerNode;

struct _HevTaskTimerNode
{
    HevRBTreeNode base;

    struct timespec expire;
    HevTask *task;
};

static HevTask fake_task = { .state = HEV_TASK_RUNNING };

static inline int
hev_task_timer_node_compare (HevTaskTimerNode *a, HevTaskTimerNode *b)
{
    if (a->expire.tv_sec < b->expire.tv_sec)
        return -1;
    if (a->expire.tv_sec > b->expire.tv_sec)
        return 1;

    if (a->expire.tv_nsec < b->expire.tv_nsec)
        return -1;
    if (a->expire.tv_nsec > b->expire.tv_nsec)
        return 1;

    if (a < b)
        return -1;
    if (a > b)
        return 1;

    return 0;
}

static inline void
hev_task_timer_get_expire (struct timespec *expire, unsigned int microseconds)
{
    if (clock_gettime (CLOCK_MONOTONIC, expire) < 0)
        abort ();

    expire->tv_sec += microseconds / 1000000;
    expire->tv_nsec += (microseconds % 1000000) * 1000;
    if (expire->tv_nsec > 1000000000L) {
        expire->tv_sec++;
        expire->tv_nsec -= 1000000000L;
    }
}

static inline unsigned int
hev_task_timer_get_time (const struct timespec *expire)
{
    struct timespec curr;
    time_t sec;
    long nsec;

    if (clock_gettime (CLOCK_MONOTONIC, &curr) < 0)
        abort ();

    sec = expire->tv_sec - curr.tv_sec;
    nsec = expire->tv_nsec - curr.tv_nsec;
    if (nsec < 0) {
        sec--;
        nsec += 1000000000L;
    }

    if (sec < 0)
        return 0;

    return (sec * 1000000) + (nsec / 1000);
}

unsigned int
hev_task_timer_wait (HevTaskTimer *self, unsigned int microseconds,
                     HevTask *task)
{
    HevTaskTimerNode curr_node, *node = &curr_node;
    HevRBTreeNode **new = &self->sort_tree.base.root, *parent = NULL;
    int leftmost = 1;

    /* get expire time */
    hev_task_timer_get_expire (&node->expire, microseconds);
    node->task = task;

    /* insert to sort tree */
    while (*new) {
        HevTaskTimerNode *this = container_of (*new, HevTaskTimerNode, base);
        int result = hev_task_timer_node_compare (node, this);

        parent = *new;
        if (result < 0) {
            new = &((*new)->left);
        } else {
            new = &((*new)->right);
            leftmost = 0;
        }
    }
    hev_rbtree_node_link (&node->base, parent, new);
    hev_rbtree_cached_insert_color (&self->sort_tree, &node->base, leftmost);

    if (leftmost) {
        /* update timer: pick current */
        if (hev_task_timer_set_time (self, &node->expire) < 0)
            abort ();
        self->sched_entity.task = task;
    }

    hev_task_yield (HEV_TASK_WAITIO);

    /* check current is first */
    if (&node->base == hev_rbtree_cached_first (&self->sort_tree))
        leftmost = 1;

    /* remove expired from sort tree */
    hev_rbtree_cached_erase (&self->sort_tree, &node->base);

    if (leftmost) {
        /* update timer: pick next */
        HevRBTreeNode *next = hev_rbtree_cached_first (&self->sort_tree);
        if (next) {
            node = container_of (next, HevTaskTimerNode, base);
            if (hev_task_timer_set_time (self, &node->expire) < 0)
                abort ();
            self->sched_entity.task = node->task;
        } else {
            self->sched_entity.task = &fake_task;
        }
    }

    /* get remaining microseconds */
    return hev_task_timer_get_time (&curr_node.expire);
}
