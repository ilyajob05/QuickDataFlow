cmake_minimum_required(VERSION 3.5)

project(Shmemq)

################# compiler config #################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") #treating warnings as errors
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Winit-self")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

################# source config #################
include_directories(include)
file(GLOB SRC_LIST "src/*.cpp")

################# library config #################


################# create executable app #################
add_executable(${CMAKE_PROJECT_NAME} main.cpp ${SRC_LIST})


################# link connect #################
target_link_libraries(${CMAKE_PROJECT_NAME} -lrt -lpthread)
