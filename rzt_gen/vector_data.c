/* generated vector source file - do not edit */
        #include "bsp_api.h"
		#include "vector_data.h"

        /* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
        #if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
			[88] = r_mtu3_tcive_interrupt, /*  */
			[93] = r_mtu4_tcive_interrupt, /*  */
						
                    };
        #endif
