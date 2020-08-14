V1.2.2
------

2020.08.12
--------

Description
-----------
- Add a new lcm server which combined the lowlevel and highlevel control. It is useful for ROS.

=============================================================================================
V1.2.1
------

2020.03.02
--------

Description
-----------
- Add new UDP constructor function as server.
- Move GetState() to GetRecv() in udp.hpp.

=============================================================================================

V1.2.0
------

2019.12.04
--------

Description
-----------
- Remove global variables in examples. All variables are user defined.
- Callback functions in LOOP need a parameter now.
- Change "PositionLimit", "PositionProtect" and "PowerProtect" to static functions.

=============================================================================================

V1.1.4
------

2019.11.20
--------

Description
-----------
- Add "PositionProtect" to prevent programs runaway.
- Move "JointLimit" to "PositionLimit".
- Move "PowerLimit" to "PowerProtect".

=============================================================================================

V1.1.3
------

2019.8.10
--------

Description
-----------
- Decrease interruptions.

Known Issues
------------
- X86 uses core 2 and 3.
- ARM(TX2) uses core 2 and 3. The core 1 and 2 of TX2 are more likely have frequency reduction.

=============================================================================================

V1.1.2
------

2019.7.4
--------

Description
-----------
- Abolish "#define", which causes conflicts between user and library macros.
  Now use "constexpr" instead. Thanks Stuart Anderson.
- Add C++11 compile option.

Known Issues
------------
- Fixed "-fPIC" related problem for shared library "liblaikago_comm.so". 

=============================================================================================

V1.1.1
------

2019.6.25
--------

Description
-----------
- Move "SetLCM" and "SetPrint" from 'class control' to 'class loop'. 
- Abolish publish or subscribe the state or command in ROS now, which causes data lag.
- Add 'print' thread.

=============================================================================================

V1.1.0
------

2019.6.1
--------

Description
-----------
- Abolish global variables, like "SendHigh, SendLow, RecvHigh, RecvLow". Now use Class.
- Replace directory "laikago_ws" with "laokago_sdk".
- Examples are now under "laokago_sdk/examples".
- Move 'liblaikago_comm.so' to folder "aliengo_sdk/lib".
- The name of send and receive variables are user defined and should be passed in.
- The name of callback functions are user defined and should be passed in.
- Replace function SetLevel() with Class Control(high or low level).
- Replace "status" with "state".

Features
--------
- Using LCM module for cross-process communication.
- Class UDP can be used to transfer user defined data now.
- Class loop can help running two or three soft-realtime threads. 
  Each thread corresponds to a callback.
- Add namespace "laikago".
- All receive functions will save data in buffer first, that's why recv callback exists.

Known Issues
------------
- Fixed 500Hz to control real robot with UDP.

=============================================================================================

V1.0.0
------

2018.12.20
----------

Description
-----------
- Simple SDK for rapid development. 
- Integrate and simplify send and receive function of UDP.
- Running with multi threads in soft-realtime mode.
