cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	if(DEFINED ENV{VITASDK})
		set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	else()
		message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	endif()
endif()

set(SHORT_NAME dave)
project(${SHORT_NAME})
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "dave")
set(VITA_TITLEID  "DAVE00001")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -DLUA_32BITS")

include_directories(
	./Lua
)

file(GLOB CFiles "Lua/*.c" "Lua/*.h")

file(GLOB CPPFiles
	"*.hpp"
	"*.h"
	"*.cpp"
)

add_executable(${SHORT_NAME}
	${CFiles}
	${CPPFiles}
)

target_link_libraries(${SHORT_NAME}
	SceDisplay_stub
	SceCtrl_stub
)

vita_create_self(${SHORT_NAME}.self ${SHORT_NAME})

vita_create_vpk(${SHORT_NAME}.vpk ${VITA_TITLEID} ${SHORT_NAME}.self
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}
)
