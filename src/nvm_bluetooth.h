#ifndef __NVM_BLUETOOTH__
#define __NVM_BLUETOOTH__

#include "nvs.h"

extern nvs_handle_t nvs_handle_bt;

/**
 * @brief Commit any pending changes to NVS.
 *
 * This function commits any changes made to the NVS namespace opened by init_nvs().
 */
void commit_nvs_changes();

/**
 * @brief Initialize Non-Volatile Storage (NVS).
 *
 * This function initializes the NVS flash partition and opens a handle to the
 * "bluetooth" namespace. If the NVS partition is truncated or a new version is found,
 * it attempts to erase and re-initialize.
 */
void init_nvs();

#endif /* __NVM_BLUETOOTH__*/