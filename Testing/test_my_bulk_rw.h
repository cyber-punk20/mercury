#ifndef TEST_MY_BULK_RW_H
#define TEST_MY_BULK_RW_H
#define NA_MY_MAX_ADDR_NAME 2048
#include "mercury.h"
#include "mercury_bulk.h"
#include "mercury_request.h"
#include "mercury_util.h"
/* Default error macro */
#include "mercury_log.h"

#include "mercury_macros.h"

extern HG_PRIVATE HG_LOG_OUTLET_DECL(hg_test);
#define HG_TEST_LOG_ERROR(...)                                                 \
    HG_LOG_WRITE(hg_test, HG_LOG_LEVEL_ERROR, __VA_ARGS__)
#define HG_TEST_LOG_WARNING(...)                                               \
    HG_LOG_WRITE(hg_test, HG_LOG_LEVEL_WARNING, __VA_ARGS__)
#ifdef HG_HAS_DEBUG
#    define HG_TEST_LOG_DEBUG(...)                                             \
        HG_LOG_WRITE(hg_test, HG_LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
#    define HG_TEST_LOG_DEBUG(...) (void) 0
#endif

/* Branch predictor hints */
#ifndef _WIN32
#    define likely(x)   __builtin_expect(!!(x), 1)
#    define unlikely(x) __builtin_expect(!!(x), 0)
#else
#    define likely(x)   (x)
#    define unlikely(x) (x)
#endif

/* Error macros */
#define HG_TEST_GOTO_DONE(label, ret, ret_val)                                 \
    do {                                                                       \
        ret = ret_val;                                                         \
        goto label;                                                            \
    } while (0)

#define HG_TEST_GOTO_ERROR(label, ret, err_val, ...)                           \
    do {                                                                       \
        HG_LOG_ERROR(__VA_ARGS__);                                             \
        ret = err_val;                                                         \
        goto label;                                                            \
    } while (0)

/* Check for hg_ret value and goto label */
#define HG_TEST_CHECK_HG_ERROR(label, hg_ret, ...)                             \
    do {                                                                       \
        if (unlikely(hg_ret != HG_SUCCESS)) {                                  \
            HG_TEST_LOG_ERROR(__VA_ARGS__);                                    \
            goto label;                                                        \
        }                                                                      \
    } while (0)

/* Check for cond, set ret to err_val and goto label */
#define HG_TEST_CHECK_ERROR(cond, label, ret, err_val, ...)                    \
    do {                                                                       \
        if (unlikely(cond)) {                                                  \
            HG_TEST_LOG_ERROR(__VA_ARGS__);                                    \
            ret = err_val;                                                     \
            goto label;                                                        \
        }                                                                      \
    } while (0)

#define HG_TEST_CHECK_ERROR_NORET(cond, label, ...)                            \
    do {                                                                       \
        if (unlikely(cond)) {                                                  \
            HG_TEST_LOG_ERROR(__VA_ARGS__);                                    \
            goto label;                                                        \
        }                                                                      \
    } while (0)

#define HG_TEST_CHECK_ERROR_DONE(cond, ...)                                    \
    do {                                                                       \
        if (unlikely(cond)) {                                                  \
            HG_TEST_LOG_ERROR(__VA_ARGS__);                                    \
        }                                                                      \
    } while (0)

/* Check for cond and print warning */
#define HG_TEST_CHECK_WARNING(cond, ...)                                       \
    do {                                                                       \
        if (unlikely(cond)) {                                                  \
            HG_TEST_LOG_WARNING(__VA_ARGS__);                                  \
        }                                                                      \
    } while (0)

#define HG_TEST(x)                                                             \
    do {                                                                       \
        printf("Testing %-62s", x);                                            \
        fflush(stdout);                                                        \
    } while (0)

#define HG_PASSED()                                                            \
    do {                                                                       \
        puts(" PASSED");                                                       \
        fflush(stdout);                                                        \
    } while (0)

#define HG_FAILED()                                                            \
    do {                                                                       \
        puts("*FAILED*");                                                      \
        fflush(stdout);                                                        \
    } while (0)

#ifdef HG_HAS_BOOST
/* Generate processor and struct for required input/output structs
 * MERCURY_GEN_PROC( struct_type_name, fields )
 */
MERCURY_GEN_PROC(my_bulk_write_in_t,
    ((hg_int32_t)(fildes))((hg_size_t)(transfer_size))((hg_size_t)(
        origin_offset))((hg_size_t)(target_offset))((hg_bulk_t)(bulk_handle)))
MERCURY_GEN_PROC(bulk_write_out_t, ((hg_size_t)(ret)))
#else
/* Define my_bulk_write_in_t */
typedef struct {
    hg_int32_t fildes;
    hg_size_t transfer_size;
    hg_size_t origin_offset;
    hg_size_t target_offset;
    hg_bulk_t bulk_handle;
} my_bulk_write_in_t;
/* Define hg_proc_bulk_write_in_t */
static HG_INLINE hg_return_t
hg_proc_my_bulk_write_in_t(hg_proc_t proc, void *data)
{
    hg_return_t ret = HG_SUCCESS;
    my_bulk_write_in_t *struct_data = (my_bulk_write_in_t *) data;

    ret = hg_proc_int32_t(proc, &struct_data->fildes);
    if (ret != HG_SUCCESS)
        return ret;

    ret = hg_proc_hg_size_t(proc, &struct_data->transfer_size);
    if (ret != HG_SUCCESS)
        return ret;

    ret = hg_proc_hg_size_t(proc, &struct_data->origin_offset);
    if (ret != HG_SUCCESS)
        return ret;

    ret = hg_proc_hg_size_t(proc, &struct_data->target_offset);
    if (ret != HG_SUCCESS)
        return ret;

    ret = hg_proc_hg_bulk_t(proc, &struct_data->bulk_handle);
    if (ret != HG_SUCCESS)
        return ret;

    return ret;
}

/* Define my_bulk_write_out_t */
typedef struct {
    hg_size_t ret;
} my_bulk_write_out_t;
/* Define hg_proc_bulk_write_out_t */
static HG_INLINE hg_return_t
hg_proc_my_bulk_write_out_t(hg_proc_t proc, void *data)
{
    hg_return_t ret = HG_SUCCESS;
    my_bulk_write_out_t *struct_data = (my_bulk_write_out_t *) data;

    ret = hg_proc_hg_size_t(proc, &struct_data->ret);
    if (ret != HG_SUCCESS)
        return ret;

    return ret;
}

#endif