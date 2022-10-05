#include "test_my_bulk_rw.h"

#include "mercury_atomic.h"
#include "mercury_time.h"
#include "mercury.h"
#include "mercury_util.h"

#include "na_mpi.h"
#include "na.h"
#include "na_test.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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



#define HG_TEST_CONFIG_FILE_NAME "/myport.cfg"
na_return_t
my_na_test_set_config(const char *addr_name, bool append)
{
    FILE *config = NULL;
    na_return_t ret;

    config = fopen(
        HG_TEST_TEMP_DIRECTORY HG_TEST_CONFIG_FILE_NAME, append ? "a" : "w");
    NA_TEST_CHECK_ERROR(config == NULL, error, ret, NA_NOENTRY,
        "Could not open config file from: %s",
        HG_TEST_TEMP_DIRECTORY HG_TEST_CONFIG_FILE_NAME);

    fprintf(config, "%s\n", addr_name);
    fclose(config);

    return NA_SUCCESS;

error:
    return ret;
}


static na_return_t
my_na_test_self_addr_publish(na_class_t *na_class, bool append, int mpi_rank)
{
    char addr_string[NA_MY_MAX_ADDR_NAME];
    size_t addr_string_len = NA_MY_MAX_ADDR_NAME;
    na_addr_t self_addr = NA_ADDR_NULL;
    na_return_t ret;

    ret = NA_Addr_self(na_class, &self_addr);
    NA_TEST_CHECK_NA_ERROR(
        error, ret, "NA_Addr_self() failed (%s)", NA_Error_to_string(ret));
    
    sprintf(addr_string, "%d: ", mpi_rank);
    ret = NA_Addr_to_string(na_class, addr_string + strlen(addr_string), &addr_string_len, self_addr);
    NA_TEST_CHECK_NA_ERROR(
        error, ret, "NA_Addr_to_string() failed (%s)", NA_Error_to_string(ret));

    ret = NA_Addr_free(na_class, self_addr);
    NA_TEST_CHECK_NA_ERROR(
        error, ret, "NA_Addr_free() failed (%s)", NA_Error_to_string(ret));

    ret = my_na_test_set_config(addr_string, append);
    NA_TEST_CHECK_NA_ERROR(error, ret, "my_na_test_set_config() failed (%s)",
        NA_Error_to_string(ret));

    return NA_SUCCESS;

error:
    if (self_addr != NA_ADDR_NULL)
        (void) NA_Addr_free(na_class, self_addr);

    return ret;
}



int
main(int argc, char *argv[]) {
    int mpi_rank, nNode;
    MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &nNode);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    // initialization
    struct hg_init_info hg_init_info = HG_INIT_INFO_INITIALIZER;
    // NA intialization
    hg_init_info.na_init_info.progress_mode = NA_NO_BLOCK;
    char *info_string = NULL, *info_string_ptr = NULL;
    info_string = (char *) malloc(sizeof(char) * NA_MY_MAX_ADDR_NAME);
    NA_TEST_CHECK_ERROR_NORET(
        info_string == NULL, error, "Could not allocate info string");
    memset(info_string, '\0', NA_MY_MAX_ADDR_NAME);
    info_string_ptr = info_string;
    info_string_ptr += sprintf(info_string_ptr, "ofi+");
    info_string_ptr += sprintf(info_string_ptr, "verbs://");
    hg_init_info.na_class = NA_Initialize_opt(info_string, false, &hg_init_info.na_init_info);
    my_na_test_self_addr_publish(hg_init_info.na_class, true, mpi_rank);
    
    hg_class_t * hg_class = HG_Init_opt(NULL, false, &hg_init_info);

}