#ifndef __BT_APP_HF_H__
#define __BT_APP_HF_H__

#include <stdint.h>
#include "esp_hf_client_api.h"


#define BT_HF_TAG               "BT_HF"

/**
 * @brief     callback function for HF client
 */
void bt_app_hf_client_cb(esp_hf_client_cb_event_t event, esp_hf_client_cb_param_t *param);
void start_hfp();
void hfp_answer_call();
void hfp_reject_call();
void hfp_send_dtmf(char code);

typedef void (*hfp_incoming_call_callback_t)(const char* number);
void hfp_register_incoming_call_callback(hfp_incoming_call_callback_t callback);

typedef void (*hfp_call_accepted_callback_t)();
void hfp_register_call_accepted_callback(hfp_call_accepted_callback_t callback);

#endif /* __BT_APP_HF_H__*/