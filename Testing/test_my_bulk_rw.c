#include "test_my_bulk_rw.h"

#include "mercury_atomic.h"
#include "mercury_time.h"
#include "mercury.h"
#include "mercury_bulk.h"
#include "mercury_request.h"
#include "mercury_util.h"



#include "na_mpi.h"
#include "na.h"
#include "na_test.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#ifdef _WIN32
#    include <Winsock2.h>
#    include <Ws2tcpip.h>
#else
#    include <arpa/inet.h>
#    include <netdb.h>
#    include <netinet/in.h>
#    include <sys/socket.h>
#    include <sys/types.h>
#    include <unistd.h>
#    if defined(HG_TEST_HAS_SYSPRCTL_H)
#        include <sys/prctl.h>
#    endif
#endif

#define MY_HG_TEST_CONFIG_FILE_NAME "/myport.cfg"
#define LOOP_NUM 10
#define TEST_BUFF_SIZE 1024 * 1024 * 1024 // 10MB
#define NDIGITS 2
#define NWIDTH  20
struct my_hg_test_bulk_args {
    hg_handle_t handle;
    hg_size_t nbytes;
    ssize_t ret;
    int fildes;
    hg_size_t transfer_size;
    hg_size_t origin_offset;
    hg_size_t target_offset;
};

int mpi_rank, nNode;
pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t done_mutex = PTHREAD_MUTEX_INITIALIZER;
int done = 0;
/* test_bulk */
hg_id_t my_hg_test_bulk_write_id_g = 0;


static HG_INLINE size_t
my_bulk_read(int fildes, const void *buf, size_t offset, size_t start_value,
    size_t nbyte, int verbose)
{
    // fprintf(stdout,  "Origin call my_bulk_read\n");
    size_t i;
    int error = 0;
    const char *buf_ptr = (const char *) buf;
    (void) fildes;

    if (verbose)
        HG_TEST_LOG_DEBUG("Executing bulk_write with fildes %d...", fildes);

    if (nbyte == 0)
        return nbyte;

    if (verbose)
        HG_TEST_LOG_DEBUG("Checking data...");

    /* Check bulk buf */
    // for (i = offset; i < nbyte + offset; i++) {
    //     if (buf_ptr[i] != (char) (i + start_value)) {
    //         HG_TEST_LOG_ERROR("Error detected in bulk transfer, buf[%zu] = %d, "
    //                           "was expecting %d!\n",
    //             i, (char) buf_ptr[i], (char) (i + start_value));
    //         // fprintf(stdout, "Error %d\n", i);
    //         error = 1;
    //         nbyte = 0;
    //         break;
    //     }
    // }
    // if (!error && verbose) {
    //     HG_TEST_LOG_DEBUG("Successfully transfered %zu bytes!", nbyte);
    //     // fprintf(stdout,  "Successfully transfered %zu bytes!\n", nbyte);
    // }
        
    return nbyte;
}


/* dedicated thread function to drive Mercury progress */
static pthread_t hg_progress_tid;
bool hg_progress_shutdown_flag = false;
static void *
hg_progress_fn(hg_context_t *hg_context)
{
    // fprintf(stdout, "rank %d: hg_progress_fn\n", mpi_rank);
    hg_return_t ret;
    unsigned int actual_count;
    
    while (!hg_progress_shutdown_flag) {
        do {
            ret = HG_Trigger(hg_context, 0, 1, &actual_count);
            // fprintf(stdout, "rank %d: hg_progress_fn(HG_Trigger)\n", mpi_rank);
        } while (
            (ret == HG_SUCCESS) && actual_count && !hg_progress_shutdown_flag);

        if (!hg_progress_shutdown_flag)
            HG_Progress(hg_context, 100);
    }

    return (NULL);
}

struct my_hg_test_perf_args {
    hg_request_t *request;
    unsigned int op_count;
    hg_atomic_int32_t op_completed_count;
};

static hg_return_t
my_hg_test_perf_forward_cb(const struct hg_cb_info *callback_info) {
    
    // fprintf(stdout, "rank %d: my_hg_test_perf_forward_cb", mpi_rank);
    // struct my_hg_test_perf_args *args =
    //     (struct my_hg_test_perf_args *) callback_info->arg;
    // if ((unsigned int) hg_atomic_incr32(&args->op_completed_count) ==
    //     args->op_count) {
    //     fprintf(stdout, "rank %d: my_hg_test_perf_forward_cb, request complete\n", mpi_rank);
    //     hg_request_complete(args->request);
    // }
    pthread_mutex_lock(&done_mutex);
    done++;
    pthread_cond_signal(&done_cond);
    pthread_mutex_unlock(&done_mutex);
        

    return HG_SUCCESS;
}
// after target pulled data
// origin read from target and check data
static hg_return_t
my_hg_test_bulk_transfer_cb(const struct hg_cb_info *hg_cb_info)
{
    //fprintf(stdout, "rank %d: Origin call my_hg_test_bulk_transfer_cb\n", mpi_rank);
    struct my_hg_test_bulk_args *bulk_args =
        (struct my_hg_test_bulk_args *) hg_cb_info->arg;
    hg_bulk_t local_bulk_handle = hg_cb_info->info.bulk.local_handle;
    hg_bulk_t origin_bulk_handle = hg_cb_info->info.bulk.origin_handle;
    hg_return_t ret = HG_SUCCESS;
    my_bulk_write_out_t out_struct;
    void *buf;
    size_t write_ret;

    if (hg_cb_info->ret == HG_CANCELED) {
        HG_TEST_LOG_DEBUG("HG_Bulk_transfer() was canceled\n");
        /* Fill output structure */
        out_struct.ret = 0;
        goto done;
    } else
        HG_TEST_CHECK_ERROR_NORET(hg_cb_info->ret != HG_SUCCESS, done,
            "Error in HG callback (%s)", HG_Error_to_string(hg_cb_info->ret));

    ret = HG_Bulk_access(local_bulk_handle, 0, bulk_args->nbytes,
        HG_BULK_READ_ONLY, 1, &buf, NULL, NULL);
    HG_TEST_CHECK_HG_ERROR(
        done, ret, "HG_Bulk_access() failed (%s)", HG_Error_to_string(ret));

    /* Call bulk_read */
    write_ret = my_bulk_read(bulk_args->fildes, buf, bulk_args->target_offset,
        bulk_args->origin_offset - bulk_args->target_offset,
        bulk_args->transfer_size, 1);

    /* Fill output structure */
    out_struct.ret = write_ret;

done:
    /* Free block handle */
    ret = HG_Bulk_free(local_bulk_handle);
    HG_TEST_CHECK_ERROR_DONE(ret != HG_SUCCESS, "HG_Bulk_free() failed (%s)",
        HG_Error_to_string(ret));

    ret = HG_Bulk_free(origin_bulk_handle);
    HG_TEST_CHECK_ERROR_DONE(ret != HG_SUCCESS, "HG_Bulk_free() failed (%s)",
        HG_Error_to_string(ret));

    /* Send response back */
    ret = HG_Respond(bulk_args->handle, NULL, NULL, &out_struct);
    HG_TEST_CHECK_ERROR_DONE(
        ret != HG_SUCCESS, "HG_Respond() failed (%s)", HG_Error_to_string(ret));

    ret = HG_Destroy(bulk_args->handle);
    HG_TEST_CHECK_ERROR_DONE(
        ret != HG_SUCCESS, "HG_Destroy() failed (%s)", HG_Error_to_string(ret));

    free(bulk_args);

    return ret;
}

// origin -> write rpc -> target
// target runs this func
hg_return_t my_hg_test_bulk_write_cb(hg_handle_t handle) {
    // fprintf(stdout, "Target calls my_hg_test_bulk_write_cb\n");
    const struct hg_info *hg_info = NULL;
    hg_bulk_t origin_bulk_handle = HG_BULK_NULL;
    hg_bulk_t local_bulk_handle = HG_BULK_NULL;
    struct my_hg_test_bulk_args *bulk_args = NULL;
    my_bulk_write_in_t in_struct;
    hg_return_t ret = HG_SUCCESS;
    int fildes;
    hg_op_id_t hg_bulk_op_id;

    bulk_args =
        (struct my_hg_test_bulk_args *) malloc(sizeof(struct my_hg_test_bulk_args));
    HG_TEST_CHECK_ERROR(bulk_args == NULL, error, ret, HG_NOMEM_ERROR,
        "Could not allocate bulk_args");

    /* Keep handle to pass to callback */
    bulk_args->handle = handle;

    /* Get info from handle */
    hg_info = HG_Get_info(handle);

    /* Get input parameters and data */
    ret = HG_Get_input(handle, &in_struct);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Get_input() failed (%s)", HG_Error_to_string(ret));

    /* Get parameters */
    fildes = in_struct.fildes;
    origin_bulk_handle = in_struct.bulk_handle;

    bulk_args->nbytes = HG_Bulk_get_size(origin_bulk_handle);
    bulk_args->transfer_size = in_struct.transfer_size;
    bulk_args->origin_offset = in_struct.origin_offset;
    bulk_args->target_offset = in_struct.target_offset;
    bulk_args->fildes = fildes;

    ret = HG_Bulk_ref_incr(origin_bulk_handle);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Bulk_ref_incr() failed (%s)", HG_Error_to_string(ret));

    /* Free input */
    ret = HG_Free_input(handle, &in_struct);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Free_input() failed (%s)", HG_Error_to_string(ret));

    /* Create a new block handle to write the data */
    ret = HG_Bulk_create(hg_info->hg_class, 1, NULL,
        (hg_size_t *) &bulk_args->nbytes, HG_BULK_READWRITE,
        &local_bulk_handle);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Bulk_create() failed (%s)", HG_Error_to_string(ret));

    /* Pull bulk data */
    HG_TEST_LOG_DEBUG("Requesting transfer_size=%" PRIu64
                      ", origin_offset=%" PRIu64 ", "
                      "target_offset=%" PRIu64,
        bulk_args->transfer_size, bulk_args->origin_offset,
        bulk_args->target_offset);
    ret = HG_Bulk_transfer_id(hg_info->context, my_hg_test_bulk_transfer_cb,
        bulk_args, HG_BULK_PULL, hg_info->addr, hg_info->context_id,
        origin_bulk_handle, bulk_args->origin_offset, local_bulk_handle,
        bulk_args->target_offset, bulk_args->transfer_size, &hg_bulk_op_id);
    // printf("HG_Bulk_transfer_id finished (%s)\n", HG_Error_to_string(ret));
    HG_TEST_CHECK_HG_ERROR(error, ret, "HG_Bulk_transfer_id() failed (%s)",
        HG_Error_to_string(ret));
    
    /* Test HG_Bulk_Cancel() */
    if (fildes < 0) {
        printf("HG_Bulk_Cancel\n");
        ret = HG_Bulk_cancel(hg_bulk_op_id);
        HG_TEST_CHECK_HG_ERROR(error, ret, "HG_Bulk_cancel() failed (%s)",
            HG_Error_to_string(ret));
    }
    // printf("rank %d: Pull bulk data finished: %s\n", mpi_rank, HG_Error_to_string(ret));
    return ret;

error:
    printf("rank %d: [Error]Pull bulk data finished: %s\n", mpi_rank, HG_Error_to_string(ret));
    ret = HG_Destroy(handle);
    HG_TEST_CHECK_ERROR_DONE(
        ret != HG_SUCCESS, "HG_Destroy() failed (%s)", HG_Error_to_string(ret));

    return ret;
}

static void
my_hg_test_register(hg_class_t *hg_class) {
    my_hg_test_bulk_write_id_g = MERCURY_REGISTER(hg_class, "my_hg_test_bulk_write",
        my_bulk_write_in_t, my_bulk_write_out_t, my_hg_test_bulk_write_cb);
    // printf("my_hg_test_bulk_write_id_g: %d\n", my_hg_test_bulk_write_id_g);
}
void clear_config() {
    FILE *config = NULL;
    hg_return_t ret;

    config = fopen(
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, "w");
     fclose(config);
}
hg_return_t
my_na_test_set_config(const char *addr_name, bool append)
{
    FILE *config = NULL;
    hg_return_t ret;

    config = fopen(
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, append ? "a" : "w");
    NA_TEST_CHECK_ERROR(config == NULL, error, ret, HG_NOENTRY,
        "Could not open config file from: %s",
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME);

    fprintf(config, "%s\n", addr_name);
    fclose(config);

    return HG_SUCCESS;

error:
    return ret;
}

static hg_return_t
my_na_test_self_addr_publish(hg_class_t *hg_class, bool append, int mpi_rank)
{
    // fprintf(stdout, "rank %d: publishing self addr to file\n", mpi_rank);
    char addr_string[NA_MY_MAX_ADDR_NAME];
    size_t addr_string_len = NA_MY_MAX_ADDR_NAME;
    hg_addr_t self_addr;
    hg_return_t ret;

    ret = HG_Addr_self(hg_class, &self_addr);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Addr_self() failed (%s)", HG_Error_to_string(ret));
    
    sprintf(addr_string, "%d#", mpi_rank);
    ret = HG_Addr_to_string(hg_class, addr_string + strlen(addr_string), &addr_string_len, self_addr);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Addr_to_string() failed (%s)", HG_Error_to_string(ret));

    ret = HG_Addr_free(hg_class, self_addr);
    HG_TEST_CHECK_HG_ERROR(
        error, ret, "HG_Addr_free() failed (%s)", HG_Error_to_string(ret));
    // fprintf(stdout, "%s\n", mpi_rank, addr_string);
    ret = my_na_test_set_config(addr_string, append);
    
    HG_TEST_CHECK_HG_ERROR(error, ret, "my_na_test_set_config() failed (%s)",
        HG_Error_to_string(ret));

    return HG_SUCCESS;

error:
    if (self_addr != HG_ADDR_NULL)
        (void) HG_Addr_free(hg_class, self_addr);

    return ret;
}


na_return_t
my_na_test_get_config(char ***addr_names, size_t len, int nNode)
{
    // printf("Read config from file\n");
    FILE *config = NULL;
    na_return_t ret;
    int rc;
    ssize_t read;
    config = fopen(HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME, "r");
    NA_TEST_CHECK_ERROR(config == NULL, error, ret, NA_NOENTRY,
        "Could not open config file from: %s",
        HG_TEST_TEMP_DIRECTORY MY_HG_TEST_CONFIG_FILE_NAME);
    (*addr_names) = (char **) malloc(sizeof(char*) * nNode);
    NA_TEST_CHECK_ERROR_NORET(addr_names == NULL, error, "Could not allocate addr_names");
    for(int i = 0; i < nNode; i++) {
        char* line = NULL;
        char* addr_name = NULL;
        read = getline(&line, &len, config);
        // fprintf(stdout, "%s\n", line);
        NA_TEST_CHECK_ERROR(
        read == -1, error, ret, NA_NOENTRY, "Could not retrieve config name");
        NA_TEST_CHECK_ERROR_NORET(line == NULL, error, "Could not allocate line");
        addr_name = (char *) malloc(sizeof(char) * len);
        NA_TEST_CHECK_ERROR_NORET(addr_name == NULL, error, "Could not allocate addr_name");
        int rank = 0;
        sscanf(line, "%d#%s", &rank, addr_name);
        /* This prevents retaining the newline, if any */
        // addr_name[strlen(addr_name) - 1] = '\0';
        addr_name[strlen(addr_name)] = '\0';
        (*addr_names)[rank] = addr_name;
        free(line);
    }
    

    rc = fclose(config);
    // printf("Finish reading config from file\n");
    config = NULL;
    NA_TEST_CHECK_ERROR(
        rc != 0, error, ret, NA_PROTOCOL_ERROR, "fclose() failed");

    return NA_SUCCESS;

error:
    if (config != NULL)
        fclose(config);
    // for(int i = 0; i < nNode; i++) {
    //     if(addr_names[i] != NULL) {
    //         free(addr_names[i]);
    //     }
    // }
    // if(addr_names != NULL) {
    //     free(addr_names);
    // }
    return ret;
}


na_return_t
my_na_test_get_target_addrs(hg_class_t* hg_class, int mpi_rank, int nNode, hg_addr_t** target_addrs) {
    // fprintf(stdout, "rank %d: Read target address from file\n", mpi_rank);
    char** target_addr_names = NULL;
    na_return_t ret = my_na_test_get_config(&target_addr_names, NA_MY_MAX_ADDR_NAME, nNode);
    (*target_addrs) =  (hg_addr_t*) malloc(sizeof(hg_addr_t) * (nNode - 1));
    int j = 0;
    for(int i = 0; i < nNode; i++) {
        // fprintf(stdout, "rank %d: HG_Addr_lookup2, target_addr_name: %s %d\n", mpi_rank, target_addr_names[i], i);
        if(i == mpi_rank) continue;
        // target_addrs[j] = (hg_addr_t) malloc(sizeof(hg_addr_t));
        (*target_addrs)[j] = HG_ADDR_NULL;
        // fprintf(stdout, "rank %d: HG_Addr_lookup2, target_addr_name: %s %x %d\n", mpi_rank, target_addr_names[i], (*target_addrs)[j], i);
        ret = HG_Addr_lookup2(hg_class, target_addr_names[i], &(*target_addrs)[j]);
        // fprintf(stdout, "rank %d: HG_Addr_lookup2, target_addr_name: %-*s %x %d, result: %s\n", mpi_rank, 50, target_addr_names[i], (*target_addrs)[j], i,
        //         HG_Error_to_string(ret));
        j++;
    }
    // fprintf(stdout, "rank %d: Finish reading target address from file\n", mpi_rank);
    // for(int i = 0; i < nNode; i++) {
    //     if(target_addr_names[i] != NULL) {
    //         free(target_addr_names[i]);
    //     }
    // }
    // if(target_addr_names != NULL) {
    //     free(target_addr_names);
    // }
    
    return ret;
}



/*---------------------------------------------------------------------------*/
static int
my_hg_test_request_progress(unsigned int timeout, void *arg)
{
    printf("rank %d: my_hg_test_request_progress\n", mpi_rank);
    if (HG_Progress((hg_context_t *) arg, timeout) != HG_SUCCESS)
        return HG_UTIL_FAIL;

    return HG_UTIL_SUCCESS;
}

/*---------------------------------------------------------------------------*/
static int
my_hg_test_request_trigger(unsigned int timeout, unsigned int *flag, void *arg)
{
    // printf("rank %d: my_hg_test_request_trigger\n", mpi_rank);
    unsigned int count = 0;

    if (HG_Trigger((hg_context_t *) arg, timeout, 1, &count) != HG_SUCCESS)
        return HG_UTIL_FAIL;

    if (flag)
        *flag = (count > 0) ? true : false;
    // printf("rank %d: my_hg_test_request_trigger[SUCCESS]\n", mpi_rank);
    return HG_UTIL_SUCCESS;
}

/*---------------------------------------------------------------------------*/

int
main(int argc, char *argv[]) {
    
    int test_size = TEST_BUFF_SIZE;
    double time_read = 0, read_bandwidth;
    double nmbytes = (double) test_size / (1024 * 1024);
    MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &nNode);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    // delete myport.cfg
    if(mpi_rank == 0) {
        clear_config();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Initialization
    struct hg_init_info hg_init_info = HG_INIT_INFO_INITIALIZER;
    // NA intialization (na_test.c)
    // hg_init_info.na_init_info.progress_mode = NA_NO_BLOCK;
    char *info_string = NULL, *info_string_ptr = NULL;
    info_string = (char *) malloc(sizeof(char) * NA_MY_MAX_ADDR_NAME);
    NA_TEST_CHECK_ERROR_NORET(info_string == NULL, error, "Could not allocate info string");
    memset(info_string, '\0', NA_MY_MAX_ADDR_NAME);
    info_string_ptr = info_string;
    info_string_ptr += sprintf(info_string_ptr, "ofi+");
    info_string_ptr += sprintf(info_string_ptr, "verbs://");
    // listen must be true
    hg_init_info.na_class = NA_Initialize_opt(info_string, true, &hg_init_info.na_init_info);
    // HG intialization (mercury_test.c)
    hg_class_t * hg_class = HG_Init_opt(NULL, true, &hg_init_info);
    hg_context_t *context = HG_Context_create(hg_class);
    /* start up thread to drive progress */
    int thread_ret = pthread_create(&hg_progress_tid, NULL, hg_progress_fn, context);
    
    my_na_test_self_addr_publish(hg_class, true, mpi_rank);
    MPI_Barrier(MPI_COMM_WORLD);
    // Register
    my_hg_test_register(hg_class);
    /* Prepare bulk_buf */
    char **bulk_bufs;
    // void ***bulk_bufs_ptr;
    size_t *buf_sizes;
    size_t nbytes = test_size;
    hg_return_t ret = HG_SUCCESS;
    bulk_bufs = (char **)malloc((nNode - 1) * sizeof(char*));
    HG_TEST_CHECK_ERROR(bulk_bufs == NULL, done, ret, HG_NOMEM_ERROR,
        "Could not allocate bulk bufs");
    // fprintf(stdout, "rank %d: Begin bulk buffer initialization\n", mpi_rank);
    for(int i = 0; i < nNode - 1; i++) {
        bulk_bufs[i] = (char *)malloc(nbytes * sizeof(char));
        HG_TEST_CHECK_ERROR(bulk_bufs[i] == NULL, done, ret, HG_NOMEM_ERROR,
        "Could not allocate bulk bufs[%d]", i);
    }
    
    for (int i = 0; i < nNode - 1; i++) {
        for(int j = 0; j < nbytes; j++) {
            bulk_bufs[i][j] = (char) j;
        }
    }
    // fprintf(stdout, "rank %d: Finish bulk buffer initialization\n", mpi_rank);
    // bulk_bufs_ptr = (void ***) &bulk_bufs;
    buf_sizes = &nbytes;
    /* Get target addrs */
    hg_addr_t* target_addrs;
    hg_return_t hg_ret = my_na_test_get_target_addrs(hg_class, mpi_rank, nNode, &target_addrs);
    /* Create handles */
    
    int nhandles = nNode - 1;
    hg_handle_t* handles = malloc(nhandles * sizeof(hg_handle_t));
    for (int i = 0; i < nhandles; i++) {
        ret = HG_Create(context, target_addrs[i],
            my_hg_test_bulk_write_id_g, &handles[i]);
        // fprintf(stdout, "rank %d: Finish creating handle[%d], result: %s\n", mpi_rank, i, HG_Error_to_string(ret));
        HG_TEST_CHECK_HG_ERROR(
            done, ret, "HG_Create() failed (%s)", HG_Error_to_string(ret));
    }
    
    /* Register memory */
    hg_bulk_t* bulk_handles = malloc(nhandles * sizeof(hg_bulk_t));
    for(int i = 0; i < nhandles; i++) {
        bulk_handles[i] = HG_BULK_NULL;
    }
    // bulk_bufs_ptr[i]
    for(int i = 0; i < nhandles; i++) {
        ret = HG_Bulk_create(hg_class, 1, (void**)&bulk_bufs[i],
        (hg_size_t *) buf_sizes, HG_BULK_READ_ONLY, &bulk_handles[i]);
        // fprintf(stdout, "rank %d: Finish creating bulk_handles[%d], result: %s\n", mpi_rank, i, HG_Error_to_string(ret));
    }
    HG_TEST_CHECK_HG_ERROR(
        done, ret, "HG_Bulk_create() failed (%s)", HG_Error_to_string(ret));
    
    /* Create request class*/
    // hg_request_class_t *request_class = hg_request_init(my_hg_test_request_progress,
    //     my_hg_test_request_trigger, context);
    // HG_TEST_CHECK_ERROR(request_class == NULL, done, ret,
    //     HG_FAULT, "Could not create request class");
    // hg_request_t *request;
    // request = hg_request_create(request_class);
    // struct my_hg_test_perf_args args;
    // hg_atomic_init32(&args.op_completed_count, 0);
    // args.op_count = nhandles;
    // args.request = request;

    MPI_Barrier(MPI_COMM_WORLD);
    hg_time_t t1, t2;
    hg_time_get_current(&t1);
    for (int i = 0; i < nhandles; i++) {
        my_bulk_write_in_t in_struct;
        /* Fill input structure */
        in_struct.fildes = 0;
        in_struct.bulk_handle = bulk_handles[i];
        in_struct.transfer_size = nbytes;
        in_struct.origin_offset = 0;
        in_struct.target_offset = 0;
        
    // again:
        // ret = HG_Forward(
        //     handles[i], my_hg_test_perf_forward_cb, &args, &in_struct);
        ret = HG_Forward(
            handles[i], my_hg_test_perf_forward_cb, NULL, &in_struct);
        // if (ret == HG_AGAIN) {
        //     hg_request_wait(request, 0, NULL);
        //     goto again;
        // }
        // fprintf(stdout, "rank %d: HG_Forward %d result: %s\n", mpi_rank, i, HG_Error_to_string(ret));
        HG_TEST_CHECK_HG_ERROR(
            done, ret, "HG_Forward() failed (%s)", HG_Error_to_string(ret));
    }
    // hg_request_wait(request, HG_MAX_IDLE_TIME, NULL);
    // fprintf(stdout, "rank %d: Wait for async calls to be finished\n", mpi_rank);
    /* wait for callbacks to finish */
    pthread_mutex_lock(&done_mutex);
    while (done < nNode - 1) {
        pthread_cond_wait(&done_cond, &done_mutex);
    }
        
    pthread_mutex_unlock(&done_mutex);
    MPI_Barrier(MPI_COMM_WORLD);
    hg_time_get_current(&t2);
    time_read += hg_time_to_double(hg_time_subtract(t2, t1));
    read_bandwidth =
        nmbytes *
        (double) (nhandles) /
        time_read;
    /* At this point we have received everything so work out the bandwidth */
    if (mpi_rank == 0) {
        fprintf(stdout, "%-*s%*s\n", 10, "# Size", NWIDTH,
                "Bandwidth (MB/s)", NWIDTH);
        fprintf(stdout, "%-*d%*.*f\n", 10, (int) nbytes, NWIDTH, NDIGITS,
            read_bandwidth);
    }
    
done:
    hg_progress_shutdown_flag = true;
error:
    free(info_string);

    return NULL;
}