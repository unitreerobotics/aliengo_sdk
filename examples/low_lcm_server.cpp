/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#include "aliengo_sdk/aliengo_sdk.hpp"
#include <math.h>

using namespace aliengo;

class CustomData
{
public:
    CustomData(): udp(LOW_CMD_LENGTH, LOW_STATE_LENGTH){}
    
    UDP udp;
    LCM mylcm;
    LowCmd cmd;
    LowState state;
};

void UDPRecv(void *param)
{
    UDP *data = (UDP *)param; 
    data->Recv();
}

void LCMRecv(void *param)
{
    LCM *data = (LCM *)param; 
    data->Recv();
}

void RobotControl(void *param) 
{
    CustomData *data = (CustomData *)param; 

    data->udp.GetRecv(data->state);
    data->mylcm.Send(data->state);
    data->mylcm.Get(data->cmd);
    data->udp.Send(data->cmd);
}

int main(void) 
{
    Control control(LOWLEVEL);
    CustomData custom;
    control.loop.SetLCM(true);
    custom.mylcm.SubscribeCmd();
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.RegistFunc("LCM/Recv", LCMRecv, &custom.mylcm);
    control.loop.Start();

    return 0; 
}
