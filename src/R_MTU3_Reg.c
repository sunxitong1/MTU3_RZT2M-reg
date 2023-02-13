#include "hal_data.h"
#include "vector_data.h"
#include "R_MTU3_Reg.h"
//#include "bsp_irq.h"



#define MTR_TC_CNT_NUM          (13612)//(12500) /* Carrier cycle time 50[us] */
#define MTR_TC_HALF_CNT_NUM     (6806)//(6250)
#define MTR_TC_4_CNT_NUM        (3403)//(3125)
#define MTR_TDEAD_CNT_NUM       (10)//(200)   /* Dead Time 1[us] */


//#define MTR_TC_4_CNT_NUM 0x1000
//#define MTR_TDEAD_CNT_NUM 200
//#define MTR_TC_HALF_CNT_NUM (MTR_TC_4_CNT_NUM >> 1)

#define MTU_TGIV3_PRIORITY_LEVEL    (2) /* The lower the value, the higher the priority of the corresponding interrupt(0-31) */
#define MTU_TCIV3_PRIORITY_LEVEL    (1) /* The lower the value, the higher the priority of the corresponding interrupt(0-31) */

uint16_t MTU3_cnt = 0;
uint16_t MTU3_compare_value = 0;



void R_MTU3_IO_int(void)
{
	R_BSP_PinAccessEnable();						// Unlock Register Write Protection

	//NONE SAFE 
	//	R_PTADR->RSELP_b[18].RS1 = 0;
	//	R_PTADR->RSELP_b[17].RS6 = 0;
	//phase U 
	R_PORT_SR->PMC_b[17].PMC7 = 1;					//
	R_PORT_SR->PFC_b[17].PFC7 = 0x00;				//MTIOC4A<-->P17-7
	R_PORT_SR->PMC_b[18].PMC0 = 1;					//
	R_PORT_SR->PFC_b[18].PFC0 = 0x00;				//MTIOC4C<-->P18-0

	//phase V 
	R_PORT_SR->PMC_b[18].PMC2 = 1;					//
	R_PORT_SR->PFC_b[18].PFC2 = 0x00;				//MTIOC4B<-->P18-2
	R_PORT_SR->PMC_b[18].PMC3 = 1;					//
	R_PORT_SR->PFC_b[18].PFC3 = 0x01;				//MTIOC4D<-->P18-3

	//phase W 
	R_PORT_SR->PMC_b[17].PMC6 = 1;					//
	R_PORT_SR->PFC_b[17].PFC6 = 0x0;				//MTIOC3B<-->P17-6
	R_PORT_SR->PMC_b[18].PMC1 = 1;					//
	R_PORT_SR->PFC_b[18].PFC1 = 0x01;				//MTIOC3D<-->P18-1


	R_BSP_PinAccessDisable();						// Lock Register Write Protection

}

void R_MTU3_Create(void)
{
    volatile unsigned long dummy;

	__asm volatile ("cpsid i"); /* Disable IRQ interrupt (Set CPSR.I bit to 1)  */
	  __asm volatile ("isb");

    /* Cancel MTU stop state in LPC */
    dummy=1u;
    R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_LPC_RESET);
    R_BSP_MODULE_START(FSP_IP_MTU3, dummy);
    dummy = BSP_MSTP_REG_FSP_IP_MTU3(dummy);
    dummy = R_MTU5->TSTR;   /* Dummy-read for the module-stop state(2) */
    R_BSP_RegisterProtectEnable(BSP_REG_PROTECT_LPC_RESET);

    /* Enable read/write to MTU registers */
    R_MTU->TRWERA_b.RWE = 1U;
    R_MTU->TRWERB_b.RWE = 1U;

    /* Stop all channels */
    R_MTU->TSTRA = 0x00;
    R_MTU->TSTRB = 0x00;

    /* Set external clock noise filter */
    R_MTU_NF->NFCRC = 0x33;   /*The noise filter for the MTCLKA MTCLKB = PCLKH/32 */

    R_MTU->TSYRA |= 0x00;   /* MTU2.TCNT operates independently */
    
    /* Channel 3-4 Complementary PWM mode 2 */
    /* -Stop count operation- */
    R_MTU->TSTRA_b.CST3 = 0;
    R_MTU->TSTRA_b.CST4 = 0;
    /* -Counter clock, counter clear source selection- */
    R_MTU3->TCR = 0x00;     /* counts on PCLKH/1, Count at rising edge, TCNT clearing disabled */
    R_MTU3->TCR2 = 0x00;    /* counts on PCLKH/1 */
    R_MTU4->TCR = 0x00;     /* counts on PCLKH/1, Count at rising edge, TCNT clearing disabled */
    R_MTU4->TCR2 = 0x00;    /* counts on PCLKH/1 */
    /* -TCNT setting- */
    R_MTU3->TCNT = MTR_TDEAD_CNT_NUM;
    R_MTU4->TCNT = 0x0000;
    /* -Inter-channel synchronization setting- */
    R_MTU->TSYRA |= 0x00;   /* MTU3.TCNT and MTU4.TCNT operates independently */
    /* -TGR setting- */
    R_MTU3->TGRB = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* U-phase output compare register */
    R_MTU4->TGRA = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* V-phase output compare register */
    R_MTU4->TGRB = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* W-phase output compare register */
    R_MTU3->TGRD = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* U-phase output buffer register */
    R_MTU4->TGRC = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* V-phase output buffer register */
    R_MTU4->TGRD = MTR_TC_4_CNT_NUM + MTR_TDEAD_CNT_NUM/2;    /* W-phase output buffer register */
    /* -Dead time, carrier cycle setting- */
    R_MTU->TDDRA = MTR_TDEAD_CNT_NUM;
    R_MTU->TCDRA = MTR_TC_HALF_CNT_NUM;
    R_MTU3->TGRA = MTR_TC_HALF_CNT_NUM + MTR_TDEAD_CNT_NUM;
    R_MTU3->TGRC = MTR_TC_HALF_CNT_NUM + MTR_TDEAD_CNT_NUM;
    /* -Enable PWM cyclic output, set PWM output level- */
    R_MTU->TOCR1A_b.TOCL = 0;   /* Write access to the TOCS, OLSN, and OLSP bits is enabled */
    R_MTU->TOCR1A_b.PSYE = 0;   /* Toggle output is disabled */
	
//	R_MTU->TOCR1A_b.TOCS = 0;   /* TOCR1 setting is selected */    
//    R_MTU->TOCR1A_b.OLSP = 0;   /* Initial output:H, Active level:L */
//    R_MTU->TOCR1A_b.OLSN = 0;   /* Initial output:H, Active level:L */


	R_MTU->TOCR1A_b.TOCS = 1;   /* TOCR1 setting is selected */

	R_MTU->TOCR2A_b.OLS1P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOCR2A_b.OLS1N = 0;   /* Initial output:H, Active level:L */

	R_MTU->TOCR2A_b.OLS2P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOCR2A_b.OLS2N = 0;   /* Initial output:H, Active level:L */

	R_MTU->TOCR2A_b.OLS3P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOCR2A_b.OLS3N = 0;   /* Initial output:H, Active level:L */

	R_MTU->TOCR2A_b.BF = 0x03; 


	R_MTU->TOLBRA_b.OLS1P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOLBRA_b.OLS1N = 0;   /* Initial output:H, Active level:L */

	R_MTU->TOLBRA_b.OLS2P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOLBRA_b.OLS2N = 0;   /* Initial output:H, Active level:L */

	R_MTU->TOLBRA_b.OLS3P = 0;   /* Initial output:H, Active level:L */
	R_MTU->TOLBRA_b.OLS3N = 0;   /* Initial output:H, Active level:L */
//

    /* -Complementary PWM mode setting- */
    R_MTU3->TMDR1_b.MD = 0xF;   /* Complementary PWM mode 3 (transfer at crest and trough) */
    R_MTU3->TMDR1_b.BFA = 1;    /* TGRA and TGRC used together for buffer operation */
    R_MTU3->TMDR1_b.BFB = 1;    /* TGRB and TGRD used together for buffer operation */

    /* A/D converter start request setting */
  //  R_MTU4->TIER_b.TTGE2 = 1;   /* A/D converter start request generation by MTU4.TCNT underflow (trough) enabled */
    /* Interrupt setting */
//    R_MTU4->TIER_b.TCIEV = 1;   /* MTU4 Overflow interrupt enable */
//	R_MTU3->TIER_b.TCIEV = 1;   /* MTU3 Overflow interrupt enable */
//		R_MTU3->TIER |=  0x10;	/* MTU3 Overflow interrupt enable */

    /* DSM trigger setting */

	    /* Configure the interrupts */
//    R_BSP_IrqDisable(VECTOR_NUMBER_TGIA3);
//    R_BSP_IrqCfg(VECTOR_NUMBER_TGIA3, MTU_TGIV3_PRIORITY_LEVEL, (NULL));
//    R_BSP_IrqEnable(VECTOR_NUMBER_TGIA3);

//	R_BSP_IrqDisable(VECTOR_NUMBER_TGIV4);
//    R_BSP_IrqCfg(VECTOR_NUMBER_TCIV4, MTU_TGIV3_PRIORITY_LEVEL, (NULL));
//    R_BSP_IrqEnable(VECTOR_NUMBER_TCIV4);

//	R_BSP_IrqDisable(VECTOR_NUMBER_TCIV3);
//    R_BSP_IrqCfg(VECTOR_NUMBER_TCIV3, MTU_TCIV3_PRIORITY_LEVEL, (NULL));
//    R_BSP_IrqEnable(VECTOR_NUMBER_TCIV3);
	

    __asm volatile ("cpsie i");
    __asm volatile ("isb");

}


void MTU3_CH3_TGIA_isr(void)
{
//	R_PORT_SR->P[19] = (uint8_t) ((R_PORT_SR->P[19]) ^ (0x40));
	uint16_t temp = 0;
	if (++MTU3_cnt >= 10000)
    {
    	temp = MTU3_cnt;// + (R_MTU->TDDRA>>1);
    	if(temp > 6250)
    	{
    		temp = 6250;
    		MTU3_cnt = 0;
    	}
    	MTU3_cnt = 0;

//		R_MTU3->TGRD = temp; 
		R_MTU4->TGRC = MTU3_compare_value; 
		R_MTU4->TGRD = MTU3_compare_value; 
		
//		R_MTU4->TGRD = temp; 
//		R_MTU3->TGRB = temp; 
//		R_MTU4->TGRA = temp; 
//		R_MTU4->TGRB = temp; 

		
    }
//    else
//		MTU3_cnt = 0;
}




