/**
 * Copyright (c) 2013-2022 UChicago Argonne, LLC and The HDF Group.
 * Copyright (c) 2022 Intel Corporation.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* Generated file. Only edit na_config.h.in. */

#ifndef NA_CONFIG_H
#define NA_CONFIG_H

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
#define NA_VERSION_MAJOR 3
/* Reflects any minor backwards compatible API or functionality addition */
#define NA_VERSION_MINOR 0
/* Reflects any backwards compatible bug fixes */
#define NA_VERSION_PATCH 0

#include <mercury_compiler_attributes.h>

/* Unused return values */
#define NA_WARN_UNUSED_RESULT HG_ATTR_WARN_UNUSED_RESULT

/* Remove warnings when plugin does not use callback arguments */
#define NA_UNUSED HG_ATTR_UNUSED

/* Alignment */
#define NA_ALIGNED(x, a) HG_ATTR_ALIGNED(x, a)

/* Packed */
#define NA_PACKED(x) HG_ATTR_PACKED(x)

/* Fallthrough */
#define NA_FALLTHROUGH HG_ATTR_FALLTHROUGH

/* Inline */
#ifdef _WIN32
#    define NA_INLINE __inline
#else
#    define NA_INLINE __inline__
#endif

/* Shared libraries */
/* #undef NA_BUILD_SHARED_LIBS */
#ifdef NA_BUILD_SHARED_LIBS
#    ifdef na_EXPORTS
#        define NA_PUBLIC HG_ATTR_ABI_EXPORT
#    else
#        define NA_PUBLIC HG_ATTR_ABI_IMPORT
#    endif
#    define NA_PRIVATE HG_ATTR_ABI_HIDDEN
#else
#    define NA_PUBLIC
#    define NA_PRIVATE
#endif

/* Build Options */
#define NA_HAS_MULTI_PROGRESS
/* #undef NA_HAS_DEBUG */

/* HWLOC */
/* #undef NA_HAS_HWLOC */

/* BMI */
/* #undef NA_HAS_BMI */

/* MPI */
/* #undef NA_HAS_MPI */
/* #undef NA_MPI_HAS_GNI_SETUP */

/* CCI */
/* #undef NA_HAS_CCI */

/* OFI */
/* #undef NA_HAS_OFI */
/* #undef NA_OFI_HAS_EXT_GNI_H */
/* #undef NA_OFI_HAS_EXT_CXI_H */
/* #undef NA_OFI_GNI_HAS_UDREG */

/* NA SM */
#define NA_HAS_SM
/* #undef NA_SM_HAS_UUID */
#define NA_SM_HAS_CMA
#define NA_SM_SHM_PREFIX "na_sm"
#define NA_SM_TMP_DIRECTORY "/tmp"

/* UCX */
/* #undef NA_HAS_UCX */
/* #undef NA_UCX_HAS_LIB_QUERY */
/* #undef NA_UCX_HAS_THREAD_MODE_NAMES */

/* PSM */
/* #undef NA_HAS_PSM */

/* PSM2 */
/* #undef NA_HAS_PSM2 */

#endif /* NA_CONFIG_H */
