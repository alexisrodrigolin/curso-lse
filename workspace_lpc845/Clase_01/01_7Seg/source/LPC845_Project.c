
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"


#define ButtonUser gp0, 4
#define gp0 GPIO, 0

int main(void) {
	GPIO_PortInit(GPIO, 1);
	GPIO_PortInit(GPIO, 0);
	gpio_pin_config_t out_config = {.outputLogic = 1 ,.pinDirection = kGPIO_DigitalOutput};
	gpio_pin_config_t in_config = {.pinDirection = kGPIO_DigitalInput};


	int segmentValue[] = {11, 13, 0, 14, 6, 10};
	for(int i=0; i<=5; i++){
			GPIO_PinInit(gp0, segmentValue[i], &out_config);}
	GPIO_PinInit(gp0, 8, &out_config);
	GPIO_PinWrite(gp0, 8, 1);

	GPIO_PinWrite(gp0, 11, 1);

	while(true){
		for(int i=0; i<=5; i++){
			GPIO_PinWrite(gp0, segmentValue[i], 0);
			for(uint32_t i =0; i <100000; i++);
			GPIO_PinWrite(gp0, segmentValue[i], 1);
		}

	}




    return 0;
}
