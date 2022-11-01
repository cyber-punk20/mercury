#include "rpc_engine.h"

#ifndef RPC_H
#    define RPC_H

/* visible API for example RPC operation */

#define BULK_TRANSFER_SIZE 512

MERCURY_GEN_PROC(my_rpc_out_t, ((int32_t)(ret)))
MERCURY_GEN_PROC(my_rpc_in_t, ((int32_t)(input_val))((hg_bulk_t)(bulk_handle)))
/* struct used to carry state of overall operation across callbacks */
struct my_rpc_state {
    hg_size_t size;
    void *buffer;
    hg_bulk_t bulk_handle;
    hg_handle_t handle;
    my_rpc_in_t in;
    int32_t value;
};

hg_id_t
my_rpc_register(void);

#endif /* RPC_H */
