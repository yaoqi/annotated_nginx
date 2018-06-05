// annotated by chrono since 2018

/*
 * Copyright (C) Igor Sysoev
 * Copyright (C) Nginx, Inc.
 */


#ifndef _NGX_SHMTX_H_INCLUDED_
#define _NGX_SHMTX_H_INCLUDED_


#include <ngx_config.h>
#include <ngx_core.h>

// 通常会有下面两个条件编译宏
// NGX_HAVE_POSIX_SEM
// NGX_HAVE_GCC_ATOMIC=>NGX_HAVE_ATOMIC_OPS

// 互斥锁使用的两个原子变量
typedef struct {

    // 锁变量
    ngx_atomic_t   lock;

    // 信号量等待变量
#if (NGX_HAVE_POSIX_SEM)
    ngx_atomic_t   wait;
#endif
} ngx_shmtx_sh_t;


typedef struct {
    // 指向ngx_shmtx_sh_t.lock
#if (NGX_HAVE_ATOMIC_OPS)
    ngx_atomic_t  *lock;

    // 使用进程间信号量

#if (NGX_HAVE_POSIX_SEM)
    // 指向ngx_shmtx_sh_t.wait
    ngx_atomic_t  *wait;

    // 是否使用信号量的标志
    ngx_uint_t     semaphore;

    // unix信号量对象
    sem_t          sem;
#endif

    // 不会使用文件锁
#else
    ngx_fd_t       fd;
    u_char        *name;
#endif

    // 类似自旋锁的等待周期
    ngx_uint_t     spin;
} ngx_shmtx_t;


ngx_int_t ngx_shmtx_create(ngx_shmtx_t *mtx, ngx_shmtx_sh_t *addr,
    u_char *name);
void ngx_shmtx_destroy(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_trylock(ngx_shmtx_t *mtx);
void ngx_shmtx_lock(ngx_shmtx_t *mtx);
void ngx_shmtx_unlock(ngx_shmtx_t *mtx);
ngx_uint_t ngx_shmtx_force_unlock(ngx_shmtx_t *mtx, ngx_pid_t pid);


#endif /* _NGX_SHMTX_H_INCLUDED_ */
