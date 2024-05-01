cmake_minimum_required(VERSION 3.10)
project(BridgeInterface)

set(CMAKE_CXX_STANDARD 17)

add_executable(BridgeInterface main.cpp BridgeInterface.cpp Logger.cpp)

target_include_directories(BridgeInterface PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Add other flags or settings if necessary

# After building the executable, set the cap_net_admin capability on it
add_custom_command(TARGET BridgeInterface POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:BridgeInterface> $<TARGET_FILE_DIR:BridgeInterface>/BridgeInterface
    COMMAND sudo setcap cap_net_admin=eip $<TARGET_FILE:BridgeInterface>
)