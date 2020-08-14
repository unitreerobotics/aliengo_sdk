/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef __ALIENGO_COMM_HPP__
#define __ALIENGO_COMM_HPP__

#include <stdint.h>

namespace aliengo 
{

	//constexpr int ROBOTCLIENT        // slave
	constexpr int ROBOTSERVER = 1;         // master
	constexpr int HIGHLEVEL = 0x00;
	constexpr int LOWLEVEL  = 0xff;
	constexpr double PosStopF = (2.146E+9f);
	constexpr double VelStopF = (16000.0f);

	typedef struct
	{
		float x;
		float y;
		float z;
	}Cartesian;

	typedef struct
	{
		float quaternion[4];   // quaternion, normalized
		float gyroscope[3];    // angular velocity （unit: radian/second)
		float acceleration[3]; // m/(s2)
		float rpy[3];          // euler angle（unit: radian）
		float temp;
	}IMU;   // when under accelerated motion, the attitude of the robot calculated by IMU will drift.

	typedef struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	}LED;   // foot led brightness: 0~255

	typedef struct
	{
		uint8_t mode;          // motor working mode 
		float position;        // current angle (unit: radian)
		float velocity;        // current velocity (unit: radian/second)
		float torque;          // current estimated output torque (unit: N.m)
		float temperature;     // current temperature (temperature conduction is slow that leads to lag)
		int iReserve;
		float fReserve;
	}MotorState;   // motor feedback

	typedef struct
	{
		uint8_t mode;               // target working mode
		float position;             // target angle (unit: radian) 
		float velocity;             // target velocity (unit: radian/second)
		float positionStiffness;    // target joint position stiffness(after gear ratio) (unit: N.m/rad )
		float velocityStiffness;    // target joint velocity stiffness(after gear ratio) (unit: N.m/(rad/s) )
		float torque;               // target output torque (unit: N.m)
	}MotorCmd;     // motor control

	typedef struct
	{
		uint8_t levelFlag;                // flag to distinguish high level or low level 
		IMU imu;
		MotorState motorState[20];
		float footForce[4];            // force sensors
		uint32_t tick;                 // reference real-time from motion controller
		uint8_t wirelessRemote[40];    // wireless commands
		uint32_t crc;
	}LowState;       // low level feedback

	typedef struct 
	{
		uint8_t levelFlag;
		MotorCmd motorCmd[20];
		LED led[4];
		uint8_t wirelessRemote[40];
		uint32_t crc;
	}LowCmd;       // low level control

	typedef struct
	{
		uint8_t levelFlag;
		uint8_t mode;
		IMU imu;
		float forwardSpeed;               // speed of move forward or backward, scale: -1~1
		float sideSpeed;                  // speed of move left or right, scale: -1~1
		float rotateSpeed;                // speed of spin left or right, scale: -1~1
		float bodyHeight;                 // body height, scale: -1~1
		float updownSpeed;                // speed of stand up or squat down, scale: -1~1
		Cartesian forwardPosition;        // (will be float type next version) front or rear displacement, an integrated number form kinematics function, usually drift
		Cartesian sidePosition;           // (will be float type next version) left or right displacement, an integrated number form kinematics function, usually drift
		Cartesian footPosition2Body[4];   // foot position relative to body
		Cartesian footSpeed2Body[4];      // foot speed relative to body
		float footForce[4];
		uint32_t tick;
		uint8_t wirelessRemote[40];
		uint32_t crc;
	}HighState;     // high level feedback

	typedef struct
	{
		uint8_t levelFlag;
		uint8_t mode;
		float forwardSpeed;                // speed of move forward or backward, scale: -1~1
		float sideSpeed;                   // speed of move left or right, scale: -1~1
		float rotateSpeed;	               // speed of spin left or right, scale: -1~1
		float bodyHeight;                  // body height, scale: -1~1
		float footRaiseHeight;             // foot up height while walking (unavailable now)
		float yaw;                         // unit: radian, scale: -1~1
		float pitch;                       // unit: radian, scale: -1~1
		float roll;                        // unit: radian, scale: -1~1
		LED led[4];
		uint8_t wirelessRemote[40];
		uint32_t crc;
	}HighCmd;       // high level control

	typedef struct     
	{
		unsigned long long TotalCount;    // total loop count
		unsigned long long SendCount;     // total send count
		unsigned long long RecvCount;     // total receive count
		unsigned long long SendError;     // total send error 
		unsigned long long FlagError;     // total flag error 
		unsigned long long RecvCRCError;  // total reveive CRC error	
		unsigned long long RecvLoseError; // total lose package count	
	}UDPState;     // UDP communication state

	constexpr int HIGH_CMD_LENGTH   = (sizeof(HighCmd));
	constexpr int HIGH_STATE_LENGTH = (sizeof(HighState));
	constexpr int LOW_CMD_LENGTH    = (sizeof(LowCmd));
	constexpr int LOW_STATE_LENGTH  = (sizeof(LowState));
	
}

#endif
