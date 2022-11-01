#include <mercury.h>
#include <mercury_bulk.h>
#include <mercury_macros.h>

#ifndef RPC_ENGINE_H
#define RPC_ENGINE_H

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

#endif /* EXAMPLE_RPC_ENGINE_H */