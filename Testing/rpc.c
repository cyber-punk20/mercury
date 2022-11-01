#include "rpc.h"

#include "mercury.h"

#include <aio.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


static hg_return_t
my_rpc_handler(hg_handle_t handle);
static hg_return_t
my_rpc_handler_bulk_cb(const struct hg_cb_info *info);
hg_id_t
my_rpc_register(void)
{
    hg_class_t *hg_class;
    hg_id_t tmp;

    hg_class = hg_engine_get_class();

    tmp = MERCURY_REGISTER(
        hg_class, "my_rpc", my_rpc_in_t, my_rpc_out_t, my_rpc_handler);

    return (tmp);
}

/* callback/handler triggered upon receipt of rpc request */
static hg_return_t
my_rpc_handler(hg_handle_t handle)
{
    int ret;
    struct my_rpc_state *my_rpc_state_p;
    const struct hg_info *hgi;

    /* set up state structure */
    my_rpc_state_p = malloc(sizeof(*my_rpc_state_p));
    assert(my_rpc_state_p);
    my_rpc_state_p->size = BULK_TRANSFER_SIZE;
    my_rpc_state_p->handle = handle;
    /* This includes allocating a target buffer for bulk transfer */
    my_rpc_state_p->buffer = calloc(1, BULK_TRANSFER_SIZE);
    assert(my_rpc_state_p->buffer);

    /* decode input */
    ret = HG_Get_input(handle, &my_rpc_state_p->in);
    assert(ret == HG_SUCCESS);

    /* register local target buffer for bulk access */
    hgi = HG_Get_info(handle);
    assert(hgi);
    ret = HG_Bulk_create(hgi->hg_class, 1, &my_rpc_state_p->buffer,
        &my_rpc_state_p->size, HG_BULK_WRITE_ONLY,
        &my_rpc_state_p->bulk_handle);
    assert(ret == 0);

    /* initiate bulk transfer from client to server */
    ret = HG_Bulk_transfer(hgi->context, my_rpc_handler_bulk_cb, my_rpc_state_p,
        HG_BULK_PULL, hgi->addr, my_rpc_state_p->in.bulk_handle, 0,
        my_rpc_state_p->bulk_handle, 0, my_rpc_state_p->size, HG_OP_ID_IGNORE);
    assert(ret == 0);
    (void) ret;

    return (0);
}


/* callback triggered upon completion of bulk transfer */
static hg_return_t
my_rpc_handler_bulk_cb(const struct hg_cb_info *info)
{
    struct my_rpc_state *my_rpc_state_p = info->arg;
    int ret;
    char filename[256];

    assert(info->ret == 0);
    my_rpc_out_t out;
    out.ret = 0;
    ret = HG_Respond(my_rpc_state_p->handle, NULL, NULL, &out);
    assert(ret == HG_SUCCESS);
    (void) ret;
    // HG_Free_Input(my_rpc_state_p->handle, &my_rpc_state_p->in);
    HG_Bulk_free(my_rpc_state_p->bulk_handle);
    HG_Destroy(my_rpc_state_p->handle);
    free(my_rpc_state_p->buffer);
    free(my_rpc_state_p);

    return;
}
