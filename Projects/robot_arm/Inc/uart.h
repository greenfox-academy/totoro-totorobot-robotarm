#ifndef __UART_H_
#define __UART_H_

#include "stm32f7xx_hal.h"
#include "cmsis_os.h"
#include "sd_card.h"
#include "servo_control.h"
#include <string.h>
#include <stdlib.h>

#define TXBUFFERSIZE        100			// transmit
#define RXBUFFERSIZE        100			// receive

typedef enum {NO_COMMAND, HELP, GET_VALUE, SET_VALUE, EXECUTE} uart_command_t;
typedef enum {NO_ATTRIB, PULSE, ANGLE, POSITION, MANUAL_CONTROL, DATA_DISP, FILE_NAME} uart_attrib_t;

typedef struct {
	uint8_t buffer[RXBUFFERSIZE];
	uint8_t* head_p;
	uint8_t* tail_p;
	uint8_t* read_p;
	uint8_t* write_p;
} circular_buffer_t;

typedef struct {
	uart_command_t command;
	uart_attrib_t attrib;
	uint8_t device_id;
	int16_t value;
	int16_t value_x;
	int16_t value_y;
	int16_t value_z;
	uint8_t error;
	char file_name[100];
} uart_command_struct_t;

uart_command_struct_t c_params;
UART_HandleTypeDef uart_handle;

volatile uint8_t char_buff;
uint8_t TX_buffer[TXBUFFERSIZE];
volatile circular_buffer_t RX_buffer;
uint8_t command_buffer[RXBUFFERSIZE];

volatile uint8_t command_in;

extern uint8_t adc_on;
extern uint8_t lcd_data_display_on;
extern uint8_t demo_on;
extern uint8_t debug;

void uart_init(void);
void UART_Error_Handler(void);
void UART_send(char* buffer);
void UART_send_help(void);
void UART_rx_thread(void const * argument);
void process_command(void);
void execute_command(void);
void execute_file(void);
void set_value(void);
void UART_send_settings(void);
void start_lcd_data_display(void);
void stop_lcd_data_display(void);
extern void start_adc_thread(void);
extern void stop_adc_thread(void);
extern void start_demo(void);
extern void stop_demo(void);


#endif /* __UART_H_ */
