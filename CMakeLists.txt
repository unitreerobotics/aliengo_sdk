cmake_minimum_required(VERSION 2.8.3)
project(aliengo_sdk)

include_directories(include)

link_directories(lib)

add_compile_options(-std=c++11)

set(EXTRA_LIBS -pthread libaliengo_comm.so lcm)

# one pc one process
add_executable(position_example examples/position_example.cpp)
target_link_libraries(position_example ${EXTRA_LIBS})

add_executable(velocity_example examples/velocity_example.cpp)
target_link_libraries(velocity_example ${EXTRA_LIBS})

add_executable(torque_example examples/torque_example.cpp)
target_link_libraries(torque_example ${EXTRA_LIBS})

add_executable(walk_example examples/walk_example.cpp)
target_link_libraries(walk_example ${EXTRA_LIBS})

# one pc multi process
add_executable(sdk_lcm_server_high examples/high_lcm_server.cpp)
target_link_libraries(sdk_lcm_server_high ${EXTRA_LIBS})

add_executable(sdk_lcm_server_low examples/low_lcm_server.cpp)
target_link_libraries(sdk_lcm_server_low ${EXTRA_LIBS})

# multi pc
# add_executable(udp_send_test examples/udp_send_test.cpp)
# target_link_libraries(udp_send_test ${EXTRA_LIBS})

# add_executable(udp_recv_test examples/udp_recv_test.cpp)
# target_link_libraries(udp_recv_test ${EXTRA_LIBS})

#combined lcm_server
add_executable(sdk_lcm_server examples/lcm_server.cpp)
target_link_libraries(sdk_lcm_server ${EXTRA_LIBS})