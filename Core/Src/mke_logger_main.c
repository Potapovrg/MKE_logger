#include "mke_logger_main.h"

extern CRC_HandleTypeDef hcrc;

extern SPI_HandleTypeDef hspi1;

extern UART_HandleTypeDef huart1;

bufferSPI spi_receive_buffer;
bufferSPI spi_transmit_buffer;

bufferSPI testbuf[4]={
		{0b00001111,0x00,50,0,0,0b00000000,0x00,0x10,0x0E,0x08,0x28,0x00,0x00,0xE9,0x00,0x00,0x00},
		{0b00001111,0x00,0,-50,0,0b00000000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
		{0b00001111,0x00,-50,0,0,0b00100000,0x00,0x10,0x0E,0x08,0x28,0x00,0x00,0xEA,0x00,0x00,0x00},
		{0b00001111,0x00,0,50,0,0b00000000,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
					};//MKE Test with consumer

uint8_t Button1, Button2, Button3;

exec_time_struct exec_time;

uint32_t seconds;
uint32_t miliseconds;

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
	if(USBH_HID_GetDeviceType(phost) == HID_MOUSE)  // if the HID is Mouse
		{
			//exec_time=stop_exec_time();
			miliseconds=TIM2->CNT;
			HID_MOUSE_Info_TypeDef *Mouse_Info;
			Mouse_Info = USBH_HID_GetMouseInfo(phost);  // Get the info
			int X_Val = Mouse_Info->x;  // get the x value
			int Y_Val = Mouse_Info->y;  // get the y value
			if (X_Val > 127) X_Val -= 255;
			if (Y_Val > 127) Y_Val -= 255;


			int len = sprintf (Uart_Buf, "X=%d, Y=%d, Button1=%d, Button2=%d, Button3=%d, Time_TIM=%d.%04d s\r\n\0", X_Val, Y_Val, \
					                                Mouse_Info->buttons[0],Mouse_Info->buttons[1], Mouse_Info->buttons[2],seconds,miliseconds);
			//int len = sprintf (Uart_Buf, "\r\n\0");
			HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);

			spi_transmit_buffer.target=0b00001001;

			spi_transmit_buffer.mouse_x=X_Val;
			spi_transmit_buffer.mouse_y=Y_Val;

			Button1 =  Mouse_Info->buttons[0];
			Button2 =  Mouse_Info->buttons[1];
			Button3 =  Mouse_Info->buttons[2];

			if (Mouse_Info->buttons[0]==1)  spi_transmit_buffer.button=spi_transmit_buffer.button|0b00000001;
			else spi_transmit_buffer.button=spi_transmit_buffer.button&0b11111110;

			if (Mouse_Info->buttons[1]==1)  spi_transmit_buffer.button=spi_transmit_buffer.button|0b00000010;
			else spi_transmit_buffer.button=spi_transmit_buffer.button&0b11111101;

			if (Mouse_Info->buttons[2]==1)  spi_transmit_buffer.button=spi_transmit_buffer.button|0b00000100;
			else spi_transmit_buffer.button=spi_transmit_buffer.button&0b11111011;


			//HAL_UART_Transmit(&huart1, (uint8_t *) &spi_transmit_buffer, sizeof(spi_transmit_buffer), 100);
			HAL_UART_Transmit(&huart1, (uint8_t *) &Uart_Buf, len, 100);
			HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_SPI_TransmitReceive(&hspi1,(uint8_t *) &spi_transmit_buffer,(uint8_t *) &spi_receive_buffer,sizeof(spi_transmit_buffer),10);
			HAL_GPIO_WritePin(CS_GPIO_Port,CS_Pin,GPIO_PIN_RESET);
			//start_exec_time();
			TIM2->CNT=0;
			seconds=0;
		}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	seconds++;
	//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
}

