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
    CustomData(): udp(HIGH_CMD_LENGTH, HIGH_STATE_LENGTH){}
    
    UDP udp;
    LCM mylcm;
    HighCmd cmd;
    HighState state;
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
    struct CustomData *data;
    data = (struct CustomData *)param;

    data->udp.GetRecv(data->state);
    data->mylcm.Send(data->state);
    data->mylcm.Get(data->cmd);
    data->udp.Send(data->cmd);
}

int main(void) 
{
    Control control(HIGHLEVEL);
    CustomData custom;
    control.loop.SetLCM(true);
    control.loop.SetLCMPeriod(4000); //4ms
    custom.mylcm.SubscribeCmd();
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.RegistFunc("LCM/Recv", LCMRecv, &custom.mylcm);
    control.loop.Start();
    return 0; 
}
