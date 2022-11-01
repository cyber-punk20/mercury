#include "rpc_engine.h"
#include "rpc.h"
#include "test_my_rpc_client.h"

#include "mercury_atomic.h"
#include "mercury_time.h"
#include "mercury.h"
#include "mercury_bulk.h"
#include "mercury_request.h"
#include "mercury_util.h"

#include "na_mpi.h"
#include "na.h"
#include "na_test.h"

#include <mpi.h> 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>


int mpi_rank, nNode;

static int done = 0;
static pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;

#define LOOP_NUM 100

#define ADDR_BUF_SIZE 64
#define ADDR_TOTAL_SIZE 96

#define MY_HG_TEST_CONFIG_FILE_NAME "/myport.cfg"

#define NDIGITS 2
#define NWIDTH  20

static hg_id_t my_rpc_id;
static void
run_my_rpc(const char *svr_addr_string, int value);
static hg_return_t
my_rpc_cb(const struct hg_cb_info *info);

void clear_config() {
    FILE *config = NULL;
    hg_return_t ret;

    config = fopen(
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, "w");
     fclose(config);
}

hg_return_t
set_config(const char *addr_name, bool append)
{
    FILE *config = NULL;
    hg_return_t ret;

    config = fopen(
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, append ? "a" : "w");
    HG_TEST_CHECK_ERROR(config == NULL, error, ret, HG_NOENTRY,
        "Could not open config file from: %s",
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME);

    fprintf(config, "%s\n", addr_name);
    fprintf(stdout, "set_config %s\n", addr_name);
    fclose(config);
    return HG_SUCCESS;
error:
    return ret;
}

hg_return_t
get_config(char addr_strings[][ADDR_BUF_SIZE]) {
    FILE *config = NULL;
    hg_return_t ret;
    ssize_t read;
    config = fopen(
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, "r");
    HG_TEST_CHECK_ERROR(config == NULL, error, ret, HG_NOENTRY,
        "Could not open config file from: %s",
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME);
    size_t len = ADDR_BUF_SIZE;
    for(int i = 0; i < nNode; i++) {
        char* line = NULL;
        read = getline(&line, &len, config);
        
        HG_TEST_CHECK_ERROR(read == -1, error, ret, HG_NOENTRY,
        "Could not retrieve config name");
        HG_TEST_CHECK_ERROR_NORET(line == NULL, error, "Could not allocate line");
        int rank = 0;
        sscanf(line, "%d#%s", &rank, addr_strings[rank]);
        /* This prevents retaining the newline, if any */
        // addr_name[strlen(addr_name) - 1] = '\0';
        // addr_strings[rank][strlen(addr_strings[rank])] = '\0';
        fprintf(stdout, "get_config %d#%s\n", rank, addr_strings[rank]);
        free(line);
    }
    fclose(config);
    return HG_SUCCESS;
error:
    return ret;
}

static void
run_my_rpc(const char *svr_addr_string, int value)
{
    hg_addr_t svr_addr;
    my_rpc_in_t in;
    const struct hg_info *hgi;
    int ret;
    struct my_rpc_state *my_rpc_state_p;

    /* address lookup. */
    hg_engine_addr_lookup(svr_addr_string, &svr_addr);

    /* set up state structure */
    my_rpc_state_p = malloc(sizeof(*my_rpc_state_p));
    my_rpc_state_p->size = BULK_TRANSFER_SIZE;
    /* This includes allocating a src buffer for bulk transfer */
    my_rpc_state_p->buffer = calloc(1, BULK_TRANSFER_SIZE);
    assert(my_rpc_state_p->buffer);
    sprintf((char *) my_rpc_state_p->buffer, "Hello world!\n");
    my_rpc_state_p->value = value;

    /* create create handle to represent this rpc operation */
    hg_engine_create_handle(svr_addr, my_rpc_id, &my_rpc_state_p->handle);

    /* register buffer for rdma/bulk access by server */
    hgi = HG_Get_info(my_rpc_state_p->handle);
    assert(hgi);
    ret = HG_Bulk_create(hgi->hg_class, 1, &my_rpc_state_p->buffer,
        &my_rpc_state_p->size, HG_BULK_READ_ONLY, &in.bulk_handle);
    my_rpc_state_p->bulk_handle = in.bulk_handle;
    assert(ret == 0);

    /* Send rpc. Note that we are also transmitting the bulk handle in the
     * input struct.  It was set above.
     */
    in.input_val = my_rpc_state_p->value;
    ret = HG_Forward(my_rpc_state_p->handle, my_rpc_cb, my_rpc_state_p, &in);
    assert(ret == 0);
    (void) ret;

    hg_engine_addr_free(svr_addr);

    return;
}

/* callback triggered upon receipt of rpc response */
static hg_return_t
my_rpc_cb(const struct hg_cb_info *info)
{
    my_rpc_out_t out;
    int ret;
    struct my_rpc_state *my_rpc_state_p = info->arg;

    assert(info->ret == HG_SUCCESS);

    /* decode response */
    ret = HG_Get_output(info->info.forward.handle, &out);
    assert(ret == 0);
    (void) ret;

    // printf("Got response ret: %d\n", out.ret);

    /* clean up resources consumed by this rpc */
    HG_Bulk_free(my_rpc_state_p->bulk_handle);
    HG_Free_output(info->info.forward.handle, &out);
    HG_Destroy(info->info.forward.handle);
    free(my_rpc_state_p->buffer);
    free(my_rpc_state_p);

    /* signal to main() that we are done */
    pthread_mutex_lock(&done_mutex);
    done++;
    pthread_cond_signal(&done_cond);
    pthread_mutex_unlock(&done_mutex);

    return HG_SUCCESS;
}


int
main() {
    const char* local_addr_string = "ofi+verbs://";
    hg_engine_init(HG_TRUE, local_addr_string);
    // write addresses into a single file
    MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &nNode);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    char buf[ADDR_BUF_SIZE] = {'\0'};
    hg_size_t buf_size = ADDR_BUF_SIZE;
    hg_engine_print_self_addr(buf, buf_size);
    char addr_string[ADDR_TOTAL_SIZE] = {'\0'};
    sprintf(addr_string,"%d#", mpi_rank);
    strcat(addr_string, buf);
    fprintf(stdout, "Gather addresses\n");
    if(mpi_rank == 0) {
        clear_config();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    {
        char* addr_strings = (mpi_rank == 0? (char*)malloc(sizeof(char) * nNode * ADDR_TOTAL_SIZE): NULL);
        MPI_Gather(addr_string, ADDR_TOTAL_SIZE, MPI_CHAR, addr_strings, ADDR_TOTAL_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
        
        if(mpi_rank == 0) {
            // sprintf(addr_strings + (nNode - 1) * ADDR_TOTAL_SIZE, addr_string);
            for(int i = 0; i < nNode; i++) {
                set_config(addr_strings + i * ADDR_TOTAL_SIZE, true);
            }
        }
        free(addr_strings);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // register function
    my_rpc_id = my_rpc_register();
    // get addresses of other nodes
    char addr_strings[nNode][ADDR_BUF_SIZE];
    get_config(addr_strings);
    hg_time_t t1, t2;
    hg_time_get_current(&t1);
    for(int i = 0; i < LOOP_NUM; i++) {
        for(int j = 0; j < nNode; j++) {
            if(j == mpi_rank) continue;
            run_my_rpc(addr_strings[j], i);
        }
    }
    pthread_mutex_lock(&done_mutex);
    while (done < (nNode - 1) * LOOP_NUM) {
        pthread_cond_wait(&done_cond, &done_mutex);
    }
        
    pthread_mutex_unlock(&done_mutex);
    MPI_Barrier(MPI_COMM_WORLD);
    hg_time_get_current(&t2);
    double time_read = hg_time_to_double(hg_time_subtract(t2, t1));
    double read_bandwidth =
        ((double) BULK_TRANSFER_SIZE) / (1024 * 1024)  *
        (double) (LOOP_NUM) * (nNode - 1) /
        time_read;
    /* At this point we have received everything so work out the bandwidth */
    if (mpi_rank == 0) {
        fprintf(stdout, "%-*s%*s\n", 10, "# Size", NWIDTH,
                "Bandwidth (MB/s)", NWIDTH);
        fprintf(stdout, "%-*d%*.*f\n", 10, (int) BULK_TRANSFER_SIZE, NWIDTH, NDIGITS,
            read_bandwidth);
    }
    hg_progress_shutdown_flag = true;
    MPI_Barrier(MPI_COMM_WORLD);
    return NULL;
}