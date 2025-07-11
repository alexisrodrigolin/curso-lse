#include "app_tasks.h"

// Cola para datos del ADC
xQueueHandle queue_adc;
// Cola para datos de luminosidad
xQueueHandle queue_lux;

// Cola para el valor de setpoint
xQueueHandle queue_setpoint;

// Semáforo para interrupción del infrarojo
xSemaphoreHandle semphr_buzz;
// Semáforo para contador
xSemaphoreHandle semphr_counter;


// Handler para la tarea de display write
TaskHandle_t handle_display;

/**
 * @brief Inicializa todos los perifericos y colas
 */
void task_init(void *params) {
	// Inicializo semáforos
	semphr_buzz = xSemaphoreCreateBinary();
	semphr_counter = xSemaphoreCreateCounting(50, 25);


	// Inicializo colas
	queue_adc = xQueueCreate(1, sizeof(adc_data_t));
	queue_lux = xQueueCreate(1, sizeof(uint16_t));
	queue_setpoint = xQueueCreate(1, sizeof(uint16_t));

	// Inicializacion de GPIO
	wrapper_gpio_init(0);
	wrapper_gpio_init(1);
	// Inicialización del LED
	wrapper_output_init((gpio_t){LED}, true);
	// Inicialización del buzzer
	wrapper_output_init((gpio_t){BUZZER}, false);
	// Inicialización del enable del CNY70
	wrapper_output_init((gpio_t){CNY70_EN}, true);
	// Configuro el ADC
	wrapper_adc_init();
	// Configuro el display
	wrapper_display_init();
	// Configuro botones
	wrapper_btn_init();
	// Configuro interrupción por flancos para el infrarojo y para el botón del user
	wrapper_gpio_enable_irq((gpio_t){CNY70}, kPINT_PinIntEnableBothEdges, cny70_callback);

	// Inicializo el PWM
	wrapper_pwm_init();
	// Inicializo I2C y Bh1750
	wrapper_i2c_init();
	wrapper_bh1750_init();


	// Elimino tarea para liberar recursos
	vTaskDelete(NULL);
}

/**
 * @brief Activa una secuencia de conversion cada 0.25 segundos
 */
void task_adc(void *params) {

	while(1) {
		// Inicio una conversion
		ADC_DoSoftwareTriggerConvSeqA(ADC0);
		// Bloqueo la tarea por 250 ms
		vTaskDelay(pdMS_TO_TICKS(250));
	}
}

/**
 * @brief Tarea que escribe un número en el display
 */
void task_display(void *params) {
	// Variable con el dato para escribir
	uint8_t data;

	while(1) {
		// Mira el dato que haya en la cola
		if(!xQueuePeek(queue_lux, &data, pdMS_TO_TICKS(100))) { continue; }
		// Muestro el número
		wrapper_display_off();
		wrapper_display_write((uint8_t)(data / 10));
		wrapper_display_on((gpio_t){COM_1});
		vTaskDelay(pdMS_TO_TICKS(10));
		wrapper_display_off();
		wrapper_display_write((uint8_t)(data % 10));
		wrapper_display_on((gpio_t){COM_2});
		vTaskDelay(pdMS_TO_TICKS(10));
	}
}

/**
 * @brief Actualiza el duty del PWM
 */
void task_pwm(void *params) {
	// Variable para guardar los datos del ADC
	adc_data_t data = {0};

	while(1) {
		// Espero a que haya datos nuevos del ADC
		xQueuePeek(queue_adc, &data, portMAX_DELAY);

		// Calculo el duty directamente a partir del potenciómetro
		uint8_t duty = (100 * data.ref_raw) / 4095;

		// Aplico el duty al LED (usando un solo color, por ejemplo el rojo)
		if(duty < 100 && duty > 0){
    		// Actualizo el ancho de pulso
    		SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_0, duty, 0);
    	}

		// Pequeña demora
		vTaskDelay(pdMS_TO_TICKS(20));}
}

/**
 * @brief Lee periodicamente el valor de intensidad luminica
 */
void task_bh1750(void *params) {
	// Valor de intensidad luminica
	uint16_t lux = 0;
	while(1) {
		// Bloqueo por 160 ms (requisito)
		vTaskDelay(pdMS_TO_TICKS(200));
		// Leo el valor de lux
		lux = wrapper_bh1750_read();
		uint16_t porcentaje = lux * 100 / 30000;
		// Muestro por consola
		xQueueOverwrite(queue_lux, &porcentaje);
		

	}
}


/**
 * @brief Tarea que parpadea el LED de acuerdo a la intensidad lumínica
 */

/**
 * @brief Tarea que hace sonar el buzzer
 */
void task_buzzer(void *params) {

	while(1) {
		// Intenta tomar el semáforo
		xSemaphoreTake(semphr_buzz, portMAX_DELAY);
		// Conmuto el buzzer
		wrapper_output_toggle((gpio_t){BUZZER});
	}
}

/**
 * @brief Tarea que decrementa el contador
 */


/**
 * @brief Tarea que manualmente controla el contador
 */
void task_counter_btns(void *params) {

	while(1) {
		// Intenta tomar el semáforo
		
		// Verifica qué pulsador se presionó
		if(wrapper_btn_get_with_debouncing_with_pull_up((gpio_t){S1_BTN})) {
			// Decrementa la cuenta del semáforo
			xSemaphoreTake(semphr_counter, 0);	
		}
		else if(wrapper_btn_get_with_debouncing_with_pull_up((gpio_t){S2_BTN})) {
			// Incrementa la cuenta del semáforo
			xSemaphoreGive(semphr_counter);
		}
		// Escribe en el display
		uint16_t data = uxSemaphoreGetCount(semphr_counter);
		xQueueOverwrite(queue_setpoint, &data);
		// Demora chica para evitar que detecte muy rápido que se presionó
		vTaskDelay(pdMS_TO_TICKS(30));

	}
}

// Mine 
void task_ShowValues(void *params){
	uint8_t porcentaje = 0;
	adc_data_t data = {0};
	uint16_t control = 0;
	while(1){
		xQueuePeek(queue_setpoint, &control, pdMS_TO_TICKS(100));
		xQueuePeek(queue_lux, &porcentaje, portMAX_DELAY);
		xQueuePeek(queue_adc, &data, portMAX_DELAY);
		TickType_t tiempoActual = xTaskGetTickCount();
		uint32_t tiempo_ms = tiempoActual * 1000;
		uint16_t duty = 100- (100.0 * data.ref_raw) / 4095.0;
		
		PRINTF("Ticks: %ld ms\n", tiempo_ms);
		PRINTF("El porcentaje luminico es: %d\n", porcentaje);
		PRINTF("El porcentaje del setpoint es: %d\n", control+25);
		PRINTF("El porcentaje del brillo del LED D1: %d\n", duty);

		vTaskDelay(500);
	}

}