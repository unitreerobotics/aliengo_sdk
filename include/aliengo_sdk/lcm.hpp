/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef _ALIENGO_LCM_HPP_
#define _ALIENGO_LCM_HPP_

#include "comm.hpp"
#include "robot_const.hpp"
#include <lcm/lcm-cpp.hpp>

namespace aliengo 
{

    constexpr char highCmdChannel[]   = "LCM_High_Cmd";
    constexpr char highStateChannel[] = "LCM_High_State";
    constexpr char lowCmdChannel[]    = "LCM_Low_Cmd";
    constexpr char lowStateChannel[]  = "LCM_Low_State";

    class HighCmdLCMHandler 
    {
    public:
        void onMsg(const lcm::ReceiveBuffer* rbuf, const std::string& channel);
    };

    class LowCmdLCMHandler 
    {
    public:
        void onMsg(const lcm::ReceiveBuffer* rbuf, const std::string& channel);
    };

    class HighStateLCMHandler 
    {
    public:
        void onMsg(const lcm::ReceiveBuffer* rbuf, const std::string& channel);
    };

    class LowStateLCMHandler 
    {
    public:
        void onMsg(const lcm::ReceiveBuffer* rbuf, const std::string& channel);
    };


    class LCM {
	public:
        lcm::LCM lcm;
        LCM();
        ~LCM();
        void SubscribeCmd();
        void SubscribeState();         // remember to call this when change control level
        int Send(HighCmd&);            // lcm send cmd
        int Send(LowCmd&);             // lcm send cmd
        int Send(HighState&);          // lcm send state
        int Send(LowState&);           // lcm send state
		int Recv();                    // directly save in buffer
        void Get(HighCmd&);
        void Get(LowCmd&);
        void Get(HighState&);
        void Get(LowState&);
    private:
        int lcmFd;
        lcm::Subscription* subLcm;
        HighStateLCMHandler highStateLCMHandler;
        LowStateLCMHandler  lowStateLCMHandler;
        HighCmdLCMHandler   highCmdLCMHandler;
        LowCmdLCMHandler    lowCmdLCMHandler;
	};

}

#endif
