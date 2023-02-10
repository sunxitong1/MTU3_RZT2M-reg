#include "hal_data.h"
#include "vector_data.h"


FSP_CPP_HEADER
void R_BSP_WarmStart(bsp_warm_start_event_t event)
BSP_PLACE_IN_SECTION(".warm_start");
FSP_CPP_FOOTER
three_phase_duty_cycle_t duty_cycle;
mtu3_counter_t period_set_l;

uint16_t duty_change_flag = 0;
uint32_t duty_set = 0;
uint32_t period_set = 0;
uint16_t pwm_enable_flag = 0;
uint16_t debugdata = 0;
uint16_t debugdata1 = 0;

#define MTR_TC_CNT_NUM          (12500) /* Carrier cycle time 50[us] */
#define MTR_TC_HALF_CNT_NUM     (6250)
#define MTR_TC_4_CNT_NUM        (3125)
#define MTR_TDEAD_CNT_NUM       (10)//(200)   /* Dead Time 1[us] */

#define MTU_TGIV3_PRIORITY_LEVEL    (2) /* The lower the value, the higher the priority of the corresponding interrupt(0-31) */
#define MTU_TCIV3_PRIORITY_LEVEL    (1) /* The lower the value, the higher the priority of the corresponding interrupt(0-31) */

/*******************************************************************************************************************//**
 * main() is generated by the FSP Configuration editor and is used to generate threads if an RTOS is used.  This function
 * is called by main() when no RTOS is used.
 **********************************************************************************************************************/

void R_MTU3_C3_Start(void);
void R_MTU3_C3_4_Enable_Output(void);
void R_MTU3_C3_4_Disable_Output(void);
void R_MTU3_Create(void);
void pwm_out_p_high(void);
void pwm_out_p_low(void);
void R_IO_Create(void);
void R_MTU3_IO_int(void);



void R_IO_Create(void)
{
	
}


void pwm_out_p_high(void)
{
	__asm volatile ("cpsid i"); /* Disable IRQ interrupt (Set CPSR.I bit to 1)  */
	__asm volatile ("isb");
	R_MTU->TOCR1A_b.OLSP = 1;   /* Initial output:H, Active level:L */
//    R_MTU->TOCR1A_b.OLSN = 0;   /* Initial output:H, Active level:L */
	__asm volatile ("cpsie i");
    __asm volatile ("isb");
}

void pwm_out_p_low(void)
{
	__asm volatile ("cpsid i"); /* Disable IRQ interrupt (Set CPSR.I bit to 1)  */
	__asm volatile ("isb");
	R_MTU->TOCR1A_b.OLSP = 0;   /* Initial output:H, Active level:L */
//    R_MTU->TOCR1A_b.OLSN = 0;   /* Initial output:H, Active level:L */
	__asm volatile ("cpsie i");
    __asm volatile ("isb");
}


void R_MTU3_C3_Start(void)
{

    //R_MTU->TSTRA_b.CST3 = 1u;   /* MTU1.TCNT performs count operation */
}

void R_MTU3_C3_4_Enable_Output(void)
{
    R_MTU->TOERA = 0xFF;    /* MTU output is enabled */
	
    //R_MTU->TCSYSTR |= 0x18;//Specifies synchronous start for MUT3.TCNT & MUT4.TCNT
    R_MTU->TSTRA = 0xC0;//Specifies synchronous start for MUT3.TCNT & MUT4.TCNT
}


void R_MTU3_C3_4_Disable_Output(void)
{
    R_MTU->TOERA = 0xC0;    /* MTU output is disabled */
	R_MTU->TSTRA = 0x00;//Specifies synchronous start for MUT3.TCNT & MUT4.TCNT
}



void hal_entry(void)
{
    /* TODO: add your own code here */
    //MSTPCRA register Bit0 Clear

	
    *((unsigned long*)0x80280308)&=0xFFFFFFFEu;
    duty_cycle.duty[0] = 0;
    duty_cycle.duty[1] = 0;
    duty_cycle.duty[2] = 0;

    duty_cycle.duty_buffer[0] = 0;
    duty_cycle.duty_buffer[1] = 0;
    duty_cycle.duty_buffer[2] = 0;
    period_set_l.tgra_counts = 0;
    period_set_l.tgrb_counts = 0;
	
//IO config	
	R_MTU3_IO_int();

//RTU3 config
	R_MTU3_Create();
	R_MTU3_C3_4_Enable_Output();
	duty_change_flag = 0;
	//R_MTU->TOLBRB_b.
	
//    R_MTU->TSTRA = 0xC0;//Specifies synchronous start for MUT3.TCNT & MUT4.TCNT
//	R_MTU->TOERA = 0xC0;

	R_MTU3_IO_int();
	R_MTU3_IO_int();
	duty_change_flag = 0;

	R_PORT_SR->PM[19] = (uint16_t) ((R_PORT_SR->PM[19]) | 0x3000);

//	pwm_out_p_low();
 //	R_MTU3_C3_4_Disable_Output();


 //   R_MTU3_THREE_PHASE_Open(&g_three_phase0_ctrl, &g_three_phase0_cfg);
 //   R_MTU3_THREE_PHASE_DutyCycleSet(&g_three_phase0_ctrl, &duty_cycle);
 //  (void) R_MTU3_THREE_PHASE_Start(&g_three_ phase0_ctrl);
    while(1)
    	{

		    if(pwm_enable_flag == 0x11)
    		{
	
				R_PORT_SR->P[19]  = (uint8_t) ((R_PORT_SR->P[19])  | (0x40));
				R_MTU->TOCR1A_b.TOCS = 1;   	/* TOCR1 setting is selected */
				R_MTU->TOLBRA_b.OLS1P = 0;	 	/* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS1N = 0;	 	/* Initial output:H, Active level:L */
				
				R_MTU->TOLBRA_b.OLS2P = 0;	 	/* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS2N = 0;	 	/* Initial output:H, Active level:L */
				
				R_MTU->TOLBRA_b.OLS3P = 0;	 	/* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS3N = 0;	 	/* Initial output:H, Active level:L */
				R_MTU->TOERA = 0x31;    		/* MTU output is enabled */

    		}
					

			
    		if(pwm_enable_flag == 2)
    		{
	
			R_PORT_SR->P[19]  = (uint8_t) ((R_PORT_SR->P[19])  | (0x40));

			R_MTU->TOCR1A_b.TOCS = 1;   /* TOCR1 setting is selected */
			R_MTU->TOLBRA_b.OLS1P = 1;	 /* Initial output:H, Active level:L */
			R_MTU->TOLBRA_b.OLS1N = 0;	 /* Initial output:H, Active level:L */
			
			R_MTU->TOLBRA_b.OLS2P = 0;	 /* Initial output:H, Active level:L */
			R_MTU->TOLBRA_b.OLS2N = 0;	 /* Initial output:H, Active level:L */
			
			R_MTU->TOLBRA_b.OLS3P = 0;	 /* Initial output:H, Active level:L */
			R_MTU->TOLBRA_b.OLS3N = 0;	 /* Initial output:H, Active level:L */
			


    		}
			else if(pwm_enable_flag == 3)//high level
			{
				R_PORT_SR->P[19]  = (uint8_t) ((R_PORT_SR->P[19]) & (0xBF));
				R_MTU->TOCR1A_b.TOCS = 1;	/* TOCR1 setting is selected */

				R_MTU->TOLBRA_b.OLS1P = 0;	 /* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS1N = 0;	 /* Initial output:H, Active level:L */
				
				R_MTU->TOLBRA_b.OLS2P = 0;	 /* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS2N = 0;	 /* Initial output:H, Active level:L */
				
				R_MTU->TOLBRA_b.OLS3P = 0;	 /* Initial output:H, Active level:L */
				R_MTU->TOLBRA_b.OLS3N = 0;	 /* Initial output:H, Active level:L */
				
			}
    	
    	}
//        {
//
//        if(pwm_enable_flag == 1)
//        {
//            pwm_enable_flag = 0;
//            R_MTU3_C3_4_Enable_Output();
//        }
//        else if(pwm_enable_flag == 4)
//        {
//            pwm_enable_flag = 0;
//            R_MTU3_C3_4_Disable_Output();
//        }
//            if(duty_change_flag == 3)
//                {
//                    duty_change_flag = 0;
//                    duty_cycle.duty[0] = duty_set;
//                    duty_cycle.duty[1] = duty_set;
//                    duty_cycle.duty[2] = duty_set;
//                    R_MTU3_THREE_PHASE_DutyCycleSet(&g_three_phase0_ctrl, &duty_cycle);
//                    //R_MTU3->TGRD = duty_set + MTR_TDEAD_CNT_NUM/2;    /* U-phase output buffer register */
//                    //R_MTU4->TGRC = duty_set + MTR_TDEAD_CNT_NUM/2;    /* V-phase output buffer register */
//                    //R_MTU4->TGRD = duty_set + MTR_TDEAD_CNT_NUM/2;    /* W-phase output buffer register */
//
//                }
//            else if(duty_change_flag == 4)
//                {
//                    duty_change_flag = 0;
//                    period_set_l.tgra_counts = period_set;
//                    period_set_l.tgrb_counts = 0;
//                    R_MTU3_PeriodSet (&g_three_phase0_ctrl, &period_set_l);
//                }
//            debugdata++;
//            if(debugdata%1000 == 0)
//                debugdata1++;
//
//
//        }

		while(1)
		{
		 	__NOP();
		}



}


/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart(bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
//        R_IOPORT_Open (&g_ioport_ctrl, &g_bsp_pin_cfg);
    }
}
