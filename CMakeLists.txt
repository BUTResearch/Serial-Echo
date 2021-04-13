cmake_minimum_required(VERSION 3.0.0)
project(Serial-Echo VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(Libs/Serial)
include_directories(Libs/EchoSerial)
include_directories(Libs/Modem)

link_directories(Libs/Serial)
link_directories(Libs/EchoSerial)
link_directories(Libs/Modem)

add_subdirectory(Libs/Serial)
add_subdirectory(Libs/EchoSerial)
add_subdirectory(Libs/Modem)

add_executable(Serial-Echo main.cpp Settings.h)

target_link_libraries(${PROJECT_NAME} serial_lib echo_serial_lib rn2483_lib sara_n210_lib pthread)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


