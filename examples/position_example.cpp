/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#include "aliengo_sdk/aliengo_sdk.hpp"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>

#include <sys/time.h>
struct timeval tbegin;
struct timeval tend;

using namespace std;
using namespace aliengo;

class CustomData
{
public:
    CustomData(): udp(LOW_CMD_LENGTH, LOW_STATE_LENGTH){}
    
    UDP udp;
    LowCmd cmd = {0};
    LowState state = {0};
    float qInit[3]={0};
    float qDes[3]={0};
    float sin_mid_q[3] = {0.0, 0.5, -1.8};
    float Kv[3] = {0};  
    float Kp[3] = {0};
    double time_consume = 0;
    int rate_count = 0;
    int sin_count = 0;
    int motiontime = 0;
};

void Print(void *param)
{
    CustomData *data = (CustomData *)param;  
    printf("%f    %f    %f\n", data->state.motorState[FR_0].position, data->state.motorState[FR_1].position, data->state.motorState[FR_2].position);
}

void UDPRecv(void *param)
{
    UDP *data = (UDP *)param;  
    data->Recv();
}

double jointLinearInterpolation(double initPos, double targetPos, double rate)
{
    double p;
    rate = std::min(std::max(rate, 0.0), 1.0);
    p = initPos*(1-rate) + targetPos*rate;
    return p;
}

void RobotControl(void *param) 
{
    struct CustomData *data;
    data = (struct CustomData *)param;

    data->motiontime++;
    data->udp.GetRecv(data->state);

    // gravity compensation
    data->cmd.motorCmd[FR_0].torque = -0.65f;
    data->cmd.motorCmd[FL_0].torque = +0.65f;
    data->cmd.motorCmd[RR_0].torque = -0.65f;
    data->cmd.motorCmd[RL_0].torque = +0.65f;

    if( data->motiontime >= 100){
        // first, get record initial position
        if( data->motiontime >= 100 && data->motiontime < 500){
            data->qInit[0] = data->state.motorState[FR_0].position;
            data->qInit[1] = data->state.motorState[FR_1].position;
            data->qInit[2] = data->state.motorState[FR_2].position;
        }
        // second, move to the origin point of a sine movement with Kp Kd
        if( data->motiontime >= 500 && data->motiontime < 1500){
            data->rate_count++;
            double rate = data->rate_count/200.0;                       // needs count to 200
            data->Kp[0] = 15.0; data->Kp[1] = 15.0; data->Kp[2] = 15.0; 
            data->Kv[0] = 1.0; data->Kv[1] = 1.0; data->Kv[2] = 1.0;
            
            data->qDes[0] = jointLinearInterpolation(data->qInit[0], data->sin_mid_q[0], rate);
            data->qDes[1] = jointLinearInterpolation(data->qInit[1], data->sin_mid_q[1], rate);
            data->qDes[2] = jointLinearInterpolation(data->qInit[2], data->sin_mid_q[2], rate);
        }
        // last, do sine wave
        if( data->motiontime >= 1700){
            data->sin_count++;
            double sin_joint1 = 0.6 * sin(3*M_PI*data->sin_count/1000.0);
            double sin_joint2 = -0.6 * sin(3*M_PI*data->sin_count/1000.0);
            data->qDes[0] = data->sin_mid_q[0];
            data->qDes[1] = data->sin_mid_q[1] + sin_joint1;
            data->qDes[2] = data->sin_mid_q[2] + sin_joint2;
        }
    
        data->cmd.motorCmd[FR_0].position = data->qDes[0];
        data->cmd.motorCmd[FR_0].velocity = 0;
        data->cmd.motorCmd[FR_0].positionStiffness = data->Kp[0];
        data->cmd.motorCmd[FR_0].velocityStiffness = data->Kv[0];
        data->cmd.motorCmd[FR_0].torque = -0.65f;

        data->cmd.motorCmd[FR_1].position = data->qDes[1];
        data->cmd.motorCmd[FR_1].velocity = 0;
        data->cmd.motorCmd[FR_1].positionStiffness = data->Kp[1];
        data->cmd.motorCmd[FR_1].velocityStiffness = data->Kv[1];
        data->cmd.motorCmd[FR_1].torque = 0.0f;

        data->cmd.motorCmd[FR_2].position =  data->qDes[2];
        data->cmd.motorCmd[FR_2].velocity = 0;
        data->cmd.motorCmd[FR_2].positionStiffness = data->Kp[2];
        data->cmd.motorCmd[FR_2].velocityStiffness = data->Kv[2];
        data->cmd.motorCmd[FR_2].torque = 0.0f;
    }

    // Control::PositionLimit(data->cmd);
    // Control::PowerProtect(data->cmd, data->state, 1);
    // Control::PositionProtect(data->cmd, data->state, 0.087);

    data->udp.Send(data->cmd);

    gettimeofday(&tend, NULL);
    data->time_consume = ((tend.tv_sec-tbegin.tv_sec)*1000000+(tend.tv_usec-tbegin.tv_usec))/1000.0;
    tbegin = tend;
}

int main(void)
{
    std::cout << "Control level is set to LOW-level." << std::endl
              << "WARNING: Make sure the robot is hung up." << std::endl
              << "Press Enter to continue..." << std::endl;
    std::cin.ignore();

    Control control(LOWLEVEL);
    CustomData custom;
    control.loop.SetPrint(true);
    control.loop.SetPrintPeriod(2000); //2ms
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.RegistFunc("PRINT", Print, &custom);
    control.loop.Start();
    return 0; 
}
