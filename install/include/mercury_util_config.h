/**
 * Copyright (c) 2013-2022 UChicago Argonne, LLC and The HDF Group.
 * Copyright (c) 2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Generated file. Only edit mercury_util_config.h.in. */

#ifndef MERCURY_UTIL_CONFIG_H
#define MERCURY_UTIL_CONFIG_H

/*************************************/
/* Public Type and Struct Definition */
/*************************************/

/* Type definitions */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/*****************/
/* Public Macros */
/*****************/

/* Reflects any major or incompatible public API changes */
#define HG_UTIL_VERSION_MAJOR 3
/* Reflects any minor backwards compatible API or functionality addition */
#define HG_UTIL_VERSION_MINOR 0
/* Reflects any backwards compatible bug fixes */
#define HG_UTIL_VERSION_PATCH 0

/* Return codes */
#define HG_UTIL_SUCCESS 0
#define HG_UTIL_FAIL    -1

#include <mercury_compiler_attributes.h>

/* Inline macro */
#ifdef _WIN32
#    define HG_UTIL_INLINE __inline
#else
#    define HG_UTIL_INLINE __inline__
#endif

/* Alignment */
#define HG_UTIL_ALIGNED(x, a) HG_ATTR_ALIGNED(x, a)

/* Check format arguments */
#define HG_UTIL_PRINTF(_fmt, _firstarg) HG_ATTR_PRINTF(_fmt, _firstarg)

/* Shared libraries */
#define HG_UTIL_BUILD_SHARED_LIBS
#ifdef HG_UTIL_BUILD_SHARED_LIBS
#    ifdef mercury_util_EXPORTS
#        define HG_UTIL_PUBLIC HG_ATTR_ABI_EXPORT
#    else
#        define HG_UTIL_PUBLIC HG_ATTR_ABI_IMPORT
#    endif
#    define HG_UTIL_PRIVATE HG_ATTR_ABI_HIDDEN
#else
#    define HG_UTIL_PUBLIC
#    define HG_UTIL_PRIVATE
#endif

/* Define if has __attribute__((constructor(priority))) */
#define HG_UTIL_HAS_ATTR_CONSTRUCTOR_PRIORITY

/* Define if has 'clock_gettime()' */
#define HG_UTIL_HAS_CLOCK_GETTIME

/* Define if has CLOCK_MONOTONIC_COARSE */
#define HG_UTIL_HAS_CLOCK_MONOTONIC_COARSE

/* Define is has debug */
/* #undef HG_UTIL_HAS_DEBUG */

/* Define if has eventfd_t type */
#define HG_UTIL_HAS_EVENTFD_T

/* Define if has colored output */
/* #undef HG_UTIL_HAS_LOG_COLOR */

/* Define if has 'pthread_condattr_setclock()' */
#define HG_UTIL_HAS_PTHREAD_CONDATTR_SETCLOCK

/* Define if has PTHREAD_MUTEX_ADAPTIVE_NP */
#define HG_UTIL_HAS_PTHREAD_MUTEX_ADAPTIVE_NP

/* Define if has pthread_spinlock_t type */
#define HG_UTIL_HAS_PTHREAD_SPINLOCK_T

/* Define if has <stdatomic.h> */
#define HG_UTIL_HAS_STDATOMIC_H

/* Define type size of atomic_long */
#define HG_UTIL_ATOMIC_LONG_WIDTH 8

/* Define if has <sys/epoll.h> */
#define HG_UTIL_HAS_SYSEPOLL_H

/* Define if has <sys/event.h> */
/* #undef HG_UTIL_HAS_SYSEVENT_H */

/* Define if has <sys/eventfd.h> */
#define HG_UTIL_HAS_SYSEVENTFD_H

/* Define if has <sys/param.h> */
#define HG_UTIL_HAS_SYSPARAM_H

/* Define if has <sys/time.h> */
#define HG_UTIL_HAS_SYSTIME_H

/* Define if has <time.h> */
#define HG_UTIL_HAS_TIME_H

#endif /* MERCURY_UTIL_CONFIG_H */
