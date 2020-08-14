/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef _UNITREE_LEGGED_LCM_SERVER_
#define _UNITREE_LEGGED_LCM_SERVER_

#include "aliengo_sdk/aliengo_sdk.hpp"

namespace aliengo
{

template <class TCmd, class TState>
class Lcm_Server{
public:
    Lcm_Server(int cmdLength, int stateLength):udp(cmdLength, stateLength){}

    UDP udp;
    LCM mylcm;
    TCmd cmd;
    TState state;
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

template <class TCmd, class TState>
void RobotControl(void *param) 
{
    Lcm_Server<TCmd, TState> *data = (Lcm_Server<TCmd, TState> *)param; 

    data->udp.GetRecv(data->state);
    data->mylcm.Send(data->state);
    data->mylcm.Get(data->cmd);
    data->udp.Send(data->cmd);
}




// // Low command Lcm Server
// class Lcm_Server_Low
// {
// public:
//     Lcm_Server_Low() : control(LOWLEVEL), udp(LOW_CMD_LENGTH, LOW_STATE_LENGTH){
//         control.InitCmdData(cmd);
//     }
//     void UDPRecv(){
//         udp.Recv();
//     }
//     void LCMRecv(){
//         mylcm.Recv();
//     }
//     void RobotControl();

//     Control control;
//     UDP udp;
//     LCM mylcm;
//     LowCmd cmd = {0};
//     LowState state = {0};
// };

// void Lcm_Server_Low::RobotControl() 
// {
//     udp.GetRecv(state);
//     mylcm.Send(state);
//     mylcm.Get(cmd);
//     udp.Send(cmd);
// }

// // High command Lcm Server
// class Lcm_Server_High
// {
// public:
//     Lcm_Server_High(): control(HIGHLEVEL), udp(HIGH_CMD_LENGTH, HIGH_STATE_LENGTH){
//         control.InitCmdData(cmd);
//     }
//     void UDPRecv(){
//         udp.Recv();
//     }
//     void LCMRecv(){
//         mylcm.Recv();
//     }
//     void RobotControl();
    
//     Control control;
//     UDP udp;
//     LCM mylcm;
//     HighCmd cmd = {0};
//     HighState state = {0};
// };

// void Lcm_Server_High::RobotControl() 
// {
//     udp.GetRecv(state);
//     mylcm.Send(state);
//     mylcm.Get(cmd);
//     udp.Send(cmd);
// }

}   //namespace aliengo
#endif