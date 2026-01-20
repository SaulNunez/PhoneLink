#include <stdint.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "bluetooth.h"
#include "hfp.h"
#include "nvm_bluetooth.h"

void app_main()
{
    init_nvs();

    bt_app_controller_init();
    //start_hfp();
}
