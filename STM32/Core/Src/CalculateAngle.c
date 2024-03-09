#include "CalculateAngle.h"
#include <math.h>
#include <string.h>

Struct_Angle Angle;
MPU6050_t MPU6050;


void CalculateAccMPU6050(MPU6050_t* MPU6050)
{
	MPU6050->acc_roll  = atan(-MPU6050->Ax / sqrt(pow(MPU6050->Ay,2) + pow(MPU6050->Az,2))) * RADIAN_TO_DEGREE;
	MPU6050->acc_pitch = atan(MPU6050->Ay / sqrt(pow(MPU6050->Ax,2) + pow(MPU6050->Az,2))) * RADIAN_TO_DEGREE;
	//	MPU6050->acc_yaw = atan(sqrt(pow(MPU6050->acc_x, 2) + pow(MPU6050->acc_y, 2)) / MPU6050->acc_z) * RADIAN_TO_DEGREE;
	//Can't use MPU6050->acc_yaw there is no reliability. It's based on my personal experimental view.
}

//static float dt = 1/200.f; //Sample rate is 200Hz
static double timer = 0.0;
//void CalculateGyroMPU6050(Struct_MPU6050* MPU6050)
//{
//	MPU6050->gyro_roll  += MPU6050->gyro_y * dt;
//	MPU6050->gyro_pitch += MPU6050->gyro_x * dt;
//	MPU6050->gyro_yaw   += MPU6050->gyro_z * dt;
//}

void CalculateCompliFilter(MPU6050_t* MPU6050)
{
	CalculateAccMPU6050(MPU6050); //Prepare Acc MPU6050 before using Complimentary Filter.

	double dt = (double)(HAL_GetTick() - timer) / 1000;
	timer = HAL_GetTick();
	static float alpha = 0.96f;
	MPU6050->AngleX = alpha*(MPU6050->Gy * dt + MPU6050->AngleX) + (1-alpha) * MPU6050->acc_roll;
	MPU6050->AngleY = alpha*(MPU6050->Gx * dt + MPU6050->AngleY) + (1-alpha) * MPU6050->acc_pitch;
	MPU6050->AngleZ = MPU6050->AngleZ + MPU6050->Gz * dt;
}
