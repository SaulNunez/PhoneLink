#include <esp_err.h>
#include <nvs.h>
#include "nvs_flash.h"
#include "esp_log.h"

static const char *TAG = "nvs_bluetooth";
nvs_handle_t nvs_handle_bt = 0;

void commit_nvs_changes(){
    // Commit written value.
    esp_err_t err = nvs_commit(nvs_handle_bt);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) committing changes to NVS!", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "Changes committed to NVS successfully!");
    }
}

void init_nvs(){
    // Initialize NVS
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    // Open NVS handle
    ESP_LOGI(TAG, "\nOpening Non-Volatile Storage (NVS) handle...");
    err = nvs_open("bluetooth", NVS_READWRITE, &nvs_handle_bt);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Error (%s) opening NVS handle!", esp_err_to_name(err));
        return;
    }
}