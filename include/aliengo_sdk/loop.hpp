/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef _ALIENGO_LOOP_HPP_
#define _ALIENGO_LOOP_HPP_

#include "comm.hpp"
#include <pthread.h>

namespace aliengo 
{

	constexpr int PRIORITY_CMD    = 99;   // real-time priority
	constexpr int PRIORITY_STATE  = 99;
	constexpr int CPU_UDP   = 3;          // cpu affinity
	constexpr int CPU_LCM   = 2;          // here use the same one because of performance restriction
	constexpr int CPU_PRINT = 0;          // cpu 0 

	typedef void(*FunctionPointer)(void* data);

	class LOOP{
	public:
		LOOP();
		~LOOP();
		void SetLCM(bool);
		void SetPrint(bool);
        void SetUDPPeriod(int);       // 2000~20000(us) => 50~500(Hz). Do not use while controlling real robot.
		void SetLCMPeriod(int);       // call max period is 50ms, or the process will exit
		void SetPrintPeriod(int); 
		int Start(void);              // fall into loop and will not return
		// regist callback function. topic is the entrance to different loop. topic = "UDP/Send", "UDP/Recv", "LCM/Recv", "PRINT".
		void RegistFunc(const char* topic, FunctionPointer pf, void* data);  // has parameter
	};

}

#endif
