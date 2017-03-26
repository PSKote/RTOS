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
	
	TM_DELAY_Init();
	
	TM_DISCO_LedInit();
	
	/* Detect proper device */
	if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS302DL) {
		/* Initialize LIS302DL */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS302DL_Sensitivity_2_3G, TM_LIS302DL_Filter_2Hz);
	} else if (TM_LIS302DL_LIS3DSH_Detect() == TM_LIS302DL_LIS3DSH_Device_LIS3DSH) {
		/* Initialize LIS3DSH */
		TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	} else {
		/* Device is not recognized */
		/* Turn on ALL leds */
		TM_DISCO_LedOn(LED_GREEN | LED_RED | LED_BLUE | LED_ORANGE);
		while (1);
	}
	
	/* Delay for 2 seconds */
	Delayms(200);

	while (1) {
		/* Read axes data from initialized accelerometer */
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes_Data);
		
		/* Turn LEDS on or off */
		/* Check X axes */
		if (Axes_Data.X > 200) {
			TM_DISCO_LedOn(LED_RED);
		} else {
			TM_DISCO_LedOff(LED_RED);
		}
		if (Axes_Data.X < -200) {
			TM_DISCO_LedOn(LED_GREEN);
		} else {
			TM_DISCO_LedOff(LED_GREEN);
		}
		/* Check Y axes */
		if (Axes_Data.Y > 200) {
			TM_DISCO_LedOn(LED_ORANGE);
		} else {
			TM_DISCO_LedOff(LED_ORANGE);
		}
		if (Axes_Data.Y < -200) {
			TM_DISCO_LedOn(LED_BLUE);
		} else {
			TM_DISCO_LedOff(LED_BLUE);
		}
	}
}
