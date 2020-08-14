/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#include "aliengo_sdk/aliengo_sdk.hpp"
#include <math.h>
#include <iostream>

using namespace aliengo;

class CustomData
{
public:
    CustomData(): udp(LOW_CMD_LENGTH, LOW_STATE_LENGTH){
        this->motiontime = 0;
        this->cmd = {0};
        this->state = {0};
    }
    
    UDP udp;
    LowCmd cmd;
    LowState state;
    int motiontime;
};

void UDPRecv(void *param)
{
    UDP *data = (UDP *)param;    
    data->Recv();
}

void RobotControl(void *param) 
{
    CustomData *data = (CustomData *)param;

    data->motiontime++;
    data->udp.GetRecv(data->state);

    // gravity compensation
    data->cmd.motorCmd[FR_0].torque = -0.65f;
    data->cmd.motorCmd[FL_0].torque = +0.65f;
    data->cmd.motorCmd[RR_0].torque = -0.65f;
    data->cmd.motorCmd[RL_0].torque = +0.65f;

    if( data->motiontime >= 500){
        float torque = (0 - data->state.motorState[FL_1].position)*10.0f + (0 - data->state.motorState[FL_1].velocity)*1.0f;
        if(torque > 5.0f) torque = 5.0f;
        if(torque < -5.0f) torque = -5.0f;

        data->cmd.motorCmd[FL_1].position = PosStopF;
        data->cmd.motorCmd[FL_1].velocity = VelStopF;
        data->cmd.motorCmd[FL_1].positionStiffness = 0;
        data->cmd.motorCmd[FL_1].velocityStiffness = 0;
        data->cmd.motorCmd[FL_1].torque = torque;
    }
    Control::PowerProtect(data->cmd, data->state, 1);

    data->udp.Send(data->cmd);
}

int main(void) 
{
    std::cout << "Control level is set to LOW-level." << std::endl
              << "WARNING: Make sure the robot is hung up." << std::endl
              << "Press Enter to continue..." << std::endl;
    std::cin.ignore();

    Control control(LOWLEVEL);
    CustomData custom;
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.Start();
    return 0; 
}
