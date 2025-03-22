#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "sdkconfig.h"
#include "esp_timer.h"

static uint8_t adv_config_done = 0;

#define ADV_CONFIG_FLAG (1 << 0)
#define SCAN_RSP_CONFIG_FLAG (1 << 1)
#define GAP_TAG "GAP_TEST"

void bt_gapp_callback()
{
}

static uint8_t test_service_uuid128[16] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    // first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0x18,
    0x0D,
    0x00,
    0x00,
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,       // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 32,
    .p_service_uuid = test_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC),
};

static esp_ble_adv_data_t scan_rsp_config = {
    .set_scan_rsp = true,
    .include_name = true,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
};

static esp_ble_adv_params_t test_adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr        =
    //.peer_addr_type   =
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x100,
    .adv_int_max = 0x100,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_RPA_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    ESP_LOGV(GAP_TAG, "GAP_EVT, event %d", event);

    switch (event)
    {
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~SCAN_RSP_CONFIG_FLAG);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~ADV_CONFIG_FLAG);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        // advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GAP_TAG, "advertising start failed, error status = %x", param->adv_start_cmpl.status);
            break;
        }
        ESP_LOGI(GAP_TAG, "advertising start success");
        break;
    case ESP_GAP_BLE_PASSKEY_REQ_EVT: /* passkey request event */
        ESP_LOGI(GAP_TAG, "ESP_GAP_BLE_PASSKEY_REQ_EVT");
        /* Call the following function to input the passkey which is displayed on the remote device */
        // esp_ble_passkey_reply(heart_rate_profile_tab[HEART_PROFILE_APP_IDX].remote_bda, true, 0x00);
        break;
    case ESP_GAP_BLE_OOB_REQ_EVT:
    {
        ESP_LOGI(GAP_TAG, "ESP_GAP_BLE_OOB_REQ_EVT");
        uint8_t tk[16] = {1}; // If you paired with OOB, both devices need to use the same tk
        esp_ble_oob_req_reply(param->ble_security.ble_req.bd_addr, tk, sizeof(tk));
        break;
    }
    case ESP_GAP_BLE_NC_REQ_EVT:
        /* The app will receive this evt when the IO has DisplayYesNO capability and the peer device IO also has DisplayYesNo capability.
        show the passkey number to the user to confirm it with the number displayed by peer device. */
        esp_ble_confirm_reply(param->ble_security.ble_req.bd_addr, true);
        // ESP_LOGI(GAP_TAG, "ESP_GAP_BLE_NC_REQ_EVT, the passkey Notify number:%" PRIu32, param->ble_security.key_notif.passkey);
        break;
    case ESP_GAP_BLE_SEC_REQ_EVT:
        /* send the positive(true) security response to the peer device to accept the security request.
        If not accept the security request, should send the security response with negative(false) accept value*/
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;
    case ESP_GAP_BLE_PASSKEY_NOTIF_EVT: /// the app will receive this evt when the IO  has Output capability and the peer device IO has Input capability.
        /// show the passkey number to the user to input it in the peer device.
        // ESP_LOGI(GAP_TAG, "The passkey Notify number:%06" PRIu32, param->ble_security.key_notif.passkey);
        break;
    case ESP_GAP_BLE_AUTH_CMPL_EVT:
    {
        esp_log_buffer_hex("addr", param->ble_security.auth_cmpl.bd_addr, ESP_BD_ADDR_LEN);
        ESP_LOGI(GAP_TAG, "pair status = %s", param->ble_security.auth_cmpl.success ? "success" : "fail");
        if (!param->ble_security.auth_cmpl.success)
        {
            ESP_LOGI(GAP_TAG, "fail reason = 0x%x", param->ble_security.auth_cmpl.fail_reason);
        }
        break;
    }
    case ESP_GAP_BLE_SET_LOCAL_PRIVACY_COMPLETE_EVT:
        if (param->local_privacy_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GAP_TAG, "config local privacy failed, error status = %x", param->local_privacy_cmpl.status);
            break;
        }

        esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
        if (ret)
        {
            ESP_LOGE(GAP_TAG, "config adv data failed, error code = %x", ret);
        }
        else
        {
            adv_config_done |= ADV_CONFIG_FLAG;
        }

        ret = esp_ble_gap_config_adv_data(&scan_rsp_config);
        if (ret)
        {
            ESP_LOGE(GAP_TAG, "config adv data failed, error code = %x", ret);
        }
        else
        {
            adv_config_done |= SCAN_RSP_CONFIG_FLAG;
        }

        break;
    default:
        break;
    }
}

void bt_app_controller_init()
{
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    esp_err_t ret;

    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    // Init bluedroid stack
    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(GAP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }

    if ((esp_ble_gap_register_callback(gap_event_handler) != ESP_OK))
    {
        ESP_LOGE(GAP_TAG, "%s gattc register error, error code = %x", __func__, ret);
        return;
    }
}