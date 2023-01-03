/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_mtu3.h"
#include "r_timer_api.h"
#include "r_mtu3_three_phase.h"
#include "r_three_phase_api.h"
FSP_HEADER
/** Timer on MTU3 Instance. */
extern const timer_instance_t g_timer1;

/** Access the MTU3 instance using these structures when calling API functions directly (::p_api is not used). */
extern mtu3_instance_ctrl_t g_timer1_ctrl;
extern const timer_cfg_t g_timer1_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** Timer on MTU3 Instance. */
extern const timer_instance_t g_timer0;

/** Access the MTU3 instance using these structures when calling API functions directly (::p_api is not used). */
extern mtu3_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef NULL
void NULL(timer_callback_args_t *p_args);
#endif
/** MTU3 Three-Phase Instance. */
extern const three_phase_instance_t g_three_phase0;

/** Access the MTU3 Three-Phase instance using these structures when calling API functions directly (::p_api is not used). */
extern mtu3_three_phase_instance_ctrl_t g_three_phase0_ctrl;
extern const three_phase_cfg_t g_three_phase0_cfg;
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
