#ifndef TEST_MY_RPC_CLIENT_H
#define TEST_MY_RPC_CLIENT_H

#include "mercury.h"
#include "mercury_bulk.h"
#include "mercury_request.h"
#include "mercury_util.h"
/* Default error macro */
#include "mercury_log.h"

#include "mercury_macros.h"



void
hg_engine_init(hg_bool_t listen, const char *local_addr);
void
hg_engine_finalize(void);
hg_class_t *
hg_engine_get_class(void);
void
hg_engine_print_self_addr(char buf[], hg_size_t buf_size);
void
hg_engine_addr_lookup(const char *name, hg_addr_t *addr);
void
hg_engine_addr_free(hg_addr_t addr);
void
hg_engine_create_handle(hg_addr_t addr, hg_id_t id, hg_handle_t *handle);


extern bool hg_progress_shutdown_flag;


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

#endif