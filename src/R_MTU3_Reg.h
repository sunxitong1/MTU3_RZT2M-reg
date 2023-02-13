

//#define MTR_TC_CNT_NUM          		(13612)//(12500) /* Carrier cycle time 50[us] */
#define MTR_TC_HALF_CNT_NUM     (10000)	//10000--10k MTU3 CLK 200M
#define MTR_TC_4_CNT_NUM        (3403)	//
#define MTR_TDEAD_CNT_NUM       (560)	//   /* Dead Time 2.8[us] */
//#define CMP_UPDATE_DIRECTLY 1

void R_PWM_Create(void);
void R_MTU3_Create(void);
void R_MTU3_IO_int(void);



