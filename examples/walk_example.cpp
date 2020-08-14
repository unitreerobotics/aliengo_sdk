/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#include "aliengo_sdk/aliengo_sdk.hpp"
#include <math.h>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace aliengo;

class CustomData
{
public:
    CustomData(): udp(HIGH_CMD_LENGTH, HIGH_STATE_LENGTH){
        this->motiontime = 0;
        this->cmd = {0};
        this->state = {0};
    }
    
    UDP udp;
    HighCmd cmd;
    HighState state;
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

    data->motiontime += 2;
    data->udp.GetRecv(data->state);
    // printf("%f\n", state.forwardSpeed);

    data->cmd.forwardSpeed = 0.0f;
    data->cmd.sideSpeed = 0.0f;
    data->cmd.rotateSpeed = 0.0f;
    data->cmd.bodyHeight = 0.0f;

    data->cmd.mode = 0;
    data->cmd.roll  = 0;
    data->cmd.pitch = 0;
    data->cmd.yaw = 0;

    if(data->motiontime<1000){
        data->cmd.mode = 1;
    }

    if(data->motiontime>1000 && data->motiontime<1500){
        data->cmd.roll = 0.5f;
    }

    if(data->motiontime>1500 && data->motiontime<2000){
        data->cmd.pitch = 0.3f;
    }

    if(data->motiontime>2000 && data->motiontime<2500){
        data->cmd.yaw = 0.3f;
    }

    if(data->motiontime>2500 && data->motiontime<3000){
        data->cmd.bodyHeight = -0.3f;
    }

    if(data->motiontime>3000 && data->motiontime<3500){
        data->cmd.bodyHeight = 0.3f;
    }

    if(data->motiontime>3500 && data->motiontime<4000){
        data->cmd.bodyHeight = 0.0f;
    }

    if(data->motiontime>4000 && data->motiontime<5000){
        data->cmd.mode = 2;
    }

    if(data->motiontime>5000 && data->motiontime<8500){
        data->cmd.forwardSpeed = 0.1f; // -1  ~ +1
    }

    if(data->motiontime>8500 && data->motiontime<12000){
        data->cmd.forwardSpeed = -0.1f; // -1  ~ +1
    }

    if(data->motiontime>12000 && data->motiontime<16000){
        data->cmd.rotateSpeed = 0.1f;   // turn
    }

    if(data->motiontime>16000 && data->motiontime<20000){
        data->cmd.rotateSpeed = -0.1f;   // turn
    }

    if(data->motiontime>20000 ){
        data->cmd.mode = 1;
    }

    data->udp.Send(data->cmd);
}

int main(void) 
{
    std::cout << "Control level is set to HIGH-level." << std::endl
              << "WARNING: Make sure the robot is standing on the ground." << std::endl
              << "Press Enter to continue..." << std::endl;
    std::cin.ignore();

    Control control(HIGHLEVEL);
    CustomData custom;
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.Start();

    return 0; 
}
