/************************************************************************
Copyright (c) 2018-2019, Unitree Robotics.Co.Ltd. All rights reserved.
Use of this source code is governed by the MPL-2.0 license, see LICENSE.
************************************************************************/

#ifndef __ALIENGO_ROBOT_CONST_HPP__
#define __ALIENGO_ROBOT_CONST_HPP__

namespace aliengo 
{

// definition of each leg and joint
constexpr int FR_ = 0;       // leg index
constexpr int FL_ = 1;
constexpr int RR_ = 2;
constexpr int RL_ = 3;

constexpr int FR_0 = 0;      // joint index
constexpr int FR_1 = 1;      
constexpr int FR_2 = 2;

constexpr int FL_0 = 3;
constexpr int FL_1 = 4;
constexpr int FL_2 = 5;

constexpr int RR_0 = 6;
constexpr int RR_1 = 7;
constexpr int RR_2 = 8;

constexpr int RL_0 = 9;
constexpr int RL_1 = 10;
constexpr int RL_2 = 11;

constexpr double Hip_max   = 1.222;    // unit:radian ( = 70   degree)
constexpr double Hip_min   = -1.222;   // unit:radian ( = -70  degree)
constexpr double Thigh_max = 6.283;    // unit:radian ( = 360  degree)
constexpr double Thigh_min = -6.283;   // unit:radian ( = -360  degree)
constexpr double Calf_max  = -0.646;   // unit:radian ( = -37  degree)
constexpr double Calf_min  = -2.775;   // unit:radian ( = -159 degree)

}

#endif
