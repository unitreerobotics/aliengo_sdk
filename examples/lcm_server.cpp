#include "aliengo_sdk/lcm_server.hpp"
#include <iostream>
#include <strings.h>

using namespace aliengo;

template <class TCmd, class TState>
void mainFunc(Control &control, Lcm_Server<TCmd, TState> &custom){
    control.loop.SetLCM(true);
    custom.mylcm.SubscribeCmd();
    control.InitCmdData(custom.cmd);
    control.loop.RegistFunc("UDP/Send", RobotControl<TCmd, TState>, &custom);
    control.loop.RegistFunc("UDP/Recv", UDPRecv, &custom.udp);
    control.loop.RegistFunc("LCM/Recv", LCMRecv, &custom.mylcm);
    control.loop.Start();
}

//argv[1]:robot name: A1 or Aliengo, not case sensitive
//argv[2]:control level: LOWLEVEL or HIGHLEVEL, not case sensitive
int main(int argc, char *argv[]) 
{
    if(argc < 2){
        std::cout << "Please enter the control level." << std::endl;
        return 1;
    }
    if(strcasecmp(argv[2], "LOWLEVEL") == 0)
    {
        Control control(LOWLEVEL);
        Lcm_Server<LowCmd, LowState> custom(LOW_CMD_LENGTH, LOW_STATE_LENGTH);
        mainFunc<LowCmd, LowState>(control, custom);
    }
    else if(strcasecmp(argv[2], "HIGHLEVEL") == 0)
    {
        Control control(HIGHLEVEL);
        Lcm_Server<HighCmd, HighState> custom(HIGH_CMD_LENGTH, HIGH_STATE_LENGTH);
        mainFunc<HighCmd, HighState>(control, custom);
    }
    else
    {
        std::cout << "Control level name error! Can only be highlevel or lowlevel(not case sensitive)" << std::endl;
        exit(-1);
    }

    return 0; 
}