#ifndef __BT_APP__
#define __BT_APP__

#include "esp_bt.h"
#include "esp_gap_ble_api.h"

#define GAP_TAG          "GAP"

/**
 * @brief Bluetooth GAP state
 */
typedef enum {
    APP_GAP_STATE_IDLE = 0,                     /*!< Idle state */
    APP_GAP_STATE_DEVICE_DISCOVERING,           /*!< Device discovery in progress */
    APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE,     /*!< Device discovery complete */
    APP_GAP_STATE_SERVICE_DISCOVERING,          /*!< Service discovery in progress */
    APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE,    /*!< Service discovery complete */
} app_gap_state_t;

/**
 * @brief Bluetooth GAP callback data
 */
typedef struct {
    bool dev_found;                                 /*!< Device found flag */
    uint8_t bdname_len;                             /*!< Bluetooth device name length */
    uint8_t eir_len;                                /*!< Extended Inquiry Response length */
    uint8_t rssi;                                   /*!< RSSI value */
    uint32_t cod;                                   /*!< Class of Device */
    uint8_t eir[ESP_BT_GAP_EIR_DATA_LEN];           /*!< Extended Inquiry Response data */
    uint8_t bdname[ESP_BT_GAP_MAX_BDNAME_LEN + 1];  /*!< Bluetooth device name */
    esp_bd_addr_t bda;                              /*!< Bluetooth device address */
    app_gap_state_t state;                          /*!< GAP state */
} app_gap_cb_t;

/**
 * @brief Initialize the Bluetooth controller and stack
 *
 * This function initializes the Bluetooth controller, enables it in Classic BT mode,
 * initializes the Bluedroid stack, enables it, and starts the application GAP startup.
 */
void bt_app_controller_init();

#endif /* __BT_APP__*/