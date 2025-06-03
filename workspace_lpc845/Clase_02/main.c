#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_swm.h"
#include "fsl_power.h"
#include "fsl_adc.h"

// Canal de ADC para el potenciometro
#define ADC_POT_CH		0
#define RedLed			GPIO, 1, 2

uint32_t g_tick = 0;

/*
 * @brief   Application entry point.
 */
void SysTick_Handler(void) {

	static uint32_t tick = 0;

	if(tick++ >= g_tick) { 
		GPIO_PinWrite(RedLed, !GPIO_PinRead(RedLed)); 
		tick = 0;
	}
}


int main(void) {

	// Inicializacion de clock
	BOARD_BootClockFRO30M();
    BOARD_InitDebugConsole();

    //Enable GPIO Ports
    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
	GPIO_PortInit(GPIO, 1);
	GPIO_PinInit(RedLed, &out_config);




    // Activo clock de matriz de conmutacion
    CLOCK_EnableClock(kCLOCK_Swm);
    // Configuro la funcion de ADC en el canal del potenciometro
    SWM_SetFixedPinSelect(SWM0, kSWM_ADC_CHN0, true);
    // Desactivo clock de matriz de conmutacion
    CLOCK_DisableClock(kCLOCK_Swm);

    // Elijo clock desde el FRO con divisor de 1 (30MHz)
    CLOCK_Select(kADC_Clk_From_Fro);
    CLOCK_SetClkDivider(kCLOCK_DivAdcClk, 1);

    // Prendo el ADC
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    // Obtengo frecuencia deseada y calibro ADC
	uint32_t frequency = CLOCK_GetFreq(kCLOCK_Fro) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
	ADC_DoSelfCalibration(ADC0, frequency);
	// Configuracion por defecto del ADC (Synchronous Mode, Clk Divider 1, Low Power Mode true, High Voltage Range)
	adc_config_t adc_config;
	ADC_GetDefaultConfig(&adc_config);
    // Habilito el ADC
	ADC_Init(ADC0, &adc_config);
	// Configuracion para las conversiones
	adc_conv_seq_config_t adc_sequence = {
		.channelMask = 1 << ADC_POT_CH,							// Elijo el canal del potenciometro
		.triggerMask = 0,										// No hay trigger por hardware
		.triggerPolarity = kADC_TriggerPolarityPositiveEdge,	// Flanco ascendente
		.enableSyncBypass = false,								// Sin bypass de trigger
		.interruptMode = kADC_InterruptForEachConversion		// Interrupciones para cada conversion
	};
	// Configuro y habilito secuencia A
	ADC_SetConvSeqAConfig(ADC0, &adc_sequence);
	ADC_EnableConvSeqA(ADC0, true);

	SysTick_Config(SystemCoreClock / 1000);


    while(1) {
    	// Resultado de conversion
    	adc_result_info_t adc_info;
    	// Inicio conversion
    	ADC_DoSoftwareTriggerConvSeqA(ADC0);
    	// Espero a terminar la conversion
		while(!ADC_GetChannelConversionResult(ADC0, ADC_POT_CH, &adc_info));
		///make convertion 4095 to ms
		g_tick = 1500 * adc_info.result / 4095 + 500;
		// GPIO_PinWrite(RedLed, !GPIO_PinRead(RedLed));
		// SysTick_Config(SystemCoreClock * (TimeCon + 0.1));
    }


    return 0;
}
