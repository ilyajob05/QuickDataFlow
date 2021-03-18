cmake_minimum_required(VERSION 3.5)
project(QickDataFlow)


################# compiler config #################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_EXTENSIONS ON)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror") #treating warnings as errors
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized -Winit-self")

set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")


################# source config #################
include_directories(include)
file(GLOB SRC_LIST "src/*.cpp")


################# library config #################


################# create executable app #################
add_executable(${CMAKE_PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/examples/example.cpp ${SRC_LIST})


################# link connect #################
target_link_libraries(${CMAKE_PROJECT_NAME} -lrt -lpthread)


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
