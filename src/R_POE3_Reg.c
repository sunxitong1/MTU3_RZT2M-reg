

/*********************************************************************************************************************
	**
* CopyrightRenesas Electronics Corporation and/or its affiliates.	All Rights Reserved.
*------10-02-2023
*------Winter Sun
*********************************************************************************************************************
	*/
#include "hal_data.h"
#include "vector_data.h"
#include "R_POE3_Reg.h"

void R_POE3_int(void)
{
	/* Set MTU3 pin function selection and high-impedance control pin selection*/
	R_POE3->M3SELR = _POE_MTU3B_PIN_P17_6 | _POE_MTU3D_PIN_P18_0; //MTIOC3B/3D Pin Select 
	R_POE3->M4SELR1 = _POE_MTU4A_PIN_P17_7 | _POE_MTU4C_PIN_P18_0; //MTIOC4A/4C Pin Select
	R_POE3->M4SELR2 = _POE_MTU4B_PIN_P18_2 | _POE_MTU4D_PIN_P18_3; //MTIOC4B/4D Pin Select


	/* Places the pins TIOC3B/3D,MTIOC4A/4C,MTIOC4B/4D in high-impedance state */
	R_POE3->POECR2 = _POE_MTU4BDZE_ENABLE | _POE_MTU4ACZE_ENABLE | _POE_MTU3BDZE_ENABLE;

	//可能不需要这句
	//	/* MTU3 and MTU4 High-Impedance POE4F Add  */ 
	//	R_POE3->POECR4 = _POE_IC2ADDMT34ZE_ENABLE;//Adds the flag to the high-impedance control conditions
	/* Set POE#0 occur PWM output level*/
	R_POE3->ALR1_b.OLSEN = 1;						/* Enable active level setting Enable*/
	R_POE3->ALR1 |= 0x001A; 						/* Complementary output */
	//	R_POE3->ALR1_b.OLSEN = 0;						/* Disable active level setting*/
	/* Input level control/status for POE#0 */
	R_POE3->ICSR1 = _POE_POE0M_SEL_8;				// POE0# pin input has been sampled 16 times low level
}


void R_POE3_IO_int(void)
{
	{
		R_BSP_PinAccessEnable();					// Unlock Register Write Protection

		//NONE SAFE 
		//	R_PTADR->RSELP_b[18].RS1 = 0;
		//	R_PTADR->RSELP_b[17].RS6 = 0;
		//phase U 
		//POE0# P17-3/P01-4
		R_PORT_SR->PMC_b[17].PMC3 = 1;				//
		R_PORT_SR->PFC_b[17].PFC3 = 0x02;			//POE0# <-->P17-3

		R_BSP_PinAccessDisable();					// Lock Register Write Protection

	}
}



