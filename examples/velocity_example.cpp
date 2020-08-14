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
        this->Tpi = 0;
        this->cmd = {0};
        this->state = {0};
    }
    
    UDP udp;
    LowCmd cmd;
    LowState state;
    int Tpi;
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
        float speed = 1 * sin(3*M_PI*data->Tpi/1000.0);
        data->cmd.motorCmd[FL_2].position = PosStopF;
        data->cmd.motorCmd[FL_2].velocity = speed;
        data->cmd.motorCmd[FL_2].positionStiffness = 0;
        data->cmd.motorCmd[FL_2].velocityStiffness = 4.0;
        data->cmd.motorCmd[FL_2].torque = 0.0f;
        data->Tpi++;
    }
    // Control::PowerProtect(data->cmd, data->state, 1);
    // Control::PositionProtect(data->cmd, data->state, 0.087);
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
