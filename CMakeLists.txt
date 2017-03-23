# cmake -G "CodeLite - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
# set(CMAKE_C_COMPILER "c:/TDM-GCC-64/bin/x86_64-w64-mingw32-gcc-5.1.0.exe")
# set(CMAKE_CXX_COMPILER "c:/TDM-GCC-64/bin/x86_64-w64-mingw32-g++.exe")
cmake_minimum_required(VERSION 2.8.11)

project(MVECTOR)

if (MINGW)
	set(CXX_FLAGS "-std=c++11 -Wall")
endif(MINGW)

set(CMAKE_CONFIGURATION_TYPES "Release")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CXX_FLAGS}")
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. ")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

set (CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/src/include/MVECTOR.hpp
    ${CMAKE_CURRENT_LIST_DIR}/src/MVECTOR.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/t00_Basics.cpp
    ${CMAKE_CURRENT_LIST_DIR}/test/t01_Threads.cpp
)

add_executable(MVECTOR ${CXX_SRCS})
target_link_libraries(MVECTOR ${LINK_OPTIONS})
