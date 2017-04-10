#include "stm32f4xx.h"
#include "defines.h"
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"

/* Accelerometer data structure */
TM_LIS302DL_LIS3DSH_t Axes_Data;

int main(void) {	
	/* Initialize system */
	SystemInit();
	
	/* Init delay */
	TM_DELAY_Init();
	
	/* Initialize LEDs */
	TM_DISCO_LedInit();
	
	/* Detect proper device */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Turn on GREEN and RED */
		TM_DISCO_LedOn(LED_GREEN | LED_RED);
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Turn on BLUE and ORANGE */
		TM_DISCO_LedOn(LED_BLUE | LED_ORANGE);
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {
		/* Device is not recognized */
		
		/* Turn on ALL leds */
		TM_DISCO_LedOn(LED_GREEN | LED_RED | LED_BLUE | LED_ORANGE);
		
		/* Infinite loop */
		while (1);
	}
	
	/* Delay for 2 seconds */
	Delayms(2000);

	while (1) {
		/* Read axes data from initialized accelerometer */
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		
		int val, i;
		//controlling one servo motor
		val = (Axes_Data.Y+2000)*20+20000;
		TM_DISCO_LedOn(LED_RED);
		for(i=0; i<val; i++);
		TM_DISCO_LedOff(LED_RED);
		for(i=0; i<val; i++);
		
		//controlling another servo motor
		val = (Axes_Data.X+2000)*20+20000;
		TM_DISCO_LedOn(LED_GREEN);
		for(i=0; i<val; i++);
		TM_DISCO_LedOff(LED_GREEN);
		for(i=0; i<val; i++);
	}
}
