cmake_minimum_required(VERSION 3.5)
project(QuickDataFlow VERSION "2.1.0")


################# compiler config #################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") #treating warnings as errors
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Winit-self")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

# SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

################# source config #################
include_directories(include)
file(GLOB SRC_LIST "src/*.cpp")

################# create lib #################
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC_LIST})

################# link connect #################
target_link_libraries(${CMAKE_PROJECT_NAME} -lrt -lpthread)

################# create executable app #################
if(BUILD_EXAMPLES)
add_executable(example ${CMAKE_CURRENT_SOURCE_DIR}/examples/example.cpp)
target_link_libraries(example ${CMAKE_PROJECT_NAME} -lrt -lpthread)

    ################# examples config #################
    # OpenCV
    find_package(OpenCV REQUIRED)
    if(OpenCV_FOUND)
        include_directories(${OpenCV_INCLUDE_DIRS})
        ################# create examples app #################
        add_executable(img_sender ${CMAKE_CURRENT_SOURCE_DIR}/examples/img_sender.cpp)
        add_executable(video_sender ${CMAKE_CURRENT_SOURCE_DIR}/examples/video_sender.cpp)
        add_executable(video_receiver ${CMAKE_CURRENT_SOURCE_DIR}/examples/video_receiver.cpp)
        ################# link connect #################
        # OpenCV
        target_link_libraries(img_sender ${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} -lrt -lpthread)
        target_link_libraries(video_sender ${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} -lrt -lpthread)
        target_link_libraries(video_receiver ${CMAKE_PROJECT_NAME} ${OpenCV_LIBS} -lrt -lpthread)
    endif()
    
endif()


################# configure for test #################
################# Add gtest environment #################
if(BUILD_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)
    include_directories(tests)
    include_directories(${GTest_INCLUDE_DIRS})

    ################# create executable test #################
    add_executable(${CMAKE_PROJECT_NAME}_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp ${SRC_LIST})
    target_link_libraries(${CMAKE_PROJECT_NAME}_test -lrt -lpthread GTest::GTest GTest::Main)
    #target_link_libraries(${CMAKE_PROJECT_NAME}_test -lrt -lpthread ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES})

    ################# create gtest targets #################
    add_test(${CMAKE_PROJECT_NAME}_test, ${CMAKE_PROJECT_NAME}_test)
endif()

################# configure for install #################
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/shm_message.hpp" DESTINATION "/usr/include/${CMAKE_PROJECT_NAME}")
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/build/libQuickDataFlow.so" DESTINATION "/usr/lib/${CMAKE_PROJECT_NAME}")

################# install python #################
find_package (Python3 COMPONENTS Interpreter Development)
if(Python3_FOUND)
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/__init__.py" DESTINATION "/usr/lib/python3/dist-packages/${CMAKE_PROJECT_NAME}")
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${CMAKE_PROJECT_NAME}/shm_message.py" DESTINATION "/usr/lib/python3/dist-packages/${CMAKE_PROJECT_NAME}")
endif()

SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "ilyajob05") #required
INCLUDE(CPack)
