/*
 * TrafficControl.c
 *
 *  Created on: Feb 11, 2024
 *      Author: ian, Morris
 */

#include "TrafficControl.h"
#include "CalculateAngle.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "LCD1602.h"

void ConditionSelect(MPU6050_t* mpu6050)
{
    if((mpu6050->AngleY>=40 && mpu6050->AngleY<=150) /*&& (mpu6050->AngleZ<=(25) && mpu6050->AngleZ>=(-25))*/)
    {
//    	uint8_t str1[]="Raising the baton vertically\n";
//    	HAL_UART_Transmit(&huart2, str1, sizeof(str1), 100);
    	uint8_t buf[2]= "1\n";
    	HAL_UART_Transmit_DMA(&huart4, buf, sizeof(buf));
  	    lcd_put_cur(1, 0);
  	    lcd_send_string("All Stop        ");
    }
    else if((mpu6050->AngleY>=(-20) && mpu6050->AngleY<=20) && (mpu6050->Gz>=60 || mpu6050->Gz<=(-60)))
    {
//    	uint8_t str1[]="Holding the baton horizontally and swing it continuously\n";
//    	HAL_UART_Transmit(&huart2, str1, sizeof(str1), 100);
    	uint8_t buf[2]= "1\n";
    	HAL_UART_Transmit_DMA(&huart4, buf, sizeof(buf));
  	    lcd_put_cur(1, 0);
  	    lcd_send_string("Go              ");
    }
    else if((mpu6050->AngleY>=(-20) && mpu6050->AngleY<=20) /*&& (mpu6050->AngleZ<=(25) && mpu6050->AngleZ>=(-25))*/)
    {
//    	uint8_t str1[]="Holding the baton horizontally\n";
//    	HAL_UART_Transmit(&huart2, str1, sizeof(str1), 100)
    	uint8_t buf[2]= "2\n";
    	HAL_UART_Transmit_DMA(&huart4, buf, sizeof(buf));
  	    lcd_put_cur(1, 0);
  	    lcd_send_string("Stop            ");
    }
}
