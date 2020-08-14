The aliengo_sdk is mainly used for communication between PC and Aliengo control board.
It also can be used in other PCs with UDP.

## Dependencies
* [Boost](http://www.boost.org) (version 1.5.4 or higher)
* [CMake](http://www.cmake.org) (version 2.8.3 or higher)
* [LCM](https://lcm-proj.github.io) (version 1.4.0 or higher)
```bash
cd lcm-x.x.x
mkdir build
cd build
cmake ../
make
sudo make install
```

## Build
```bash
mkdir build
cd build
cmake ../
make
```

## Usage
Run examples with 'sudo' for memory locking.

### On PC
* Run the 4 examples under examples/

### With ROS
* Please follow the readme of 'unitree_ros'.