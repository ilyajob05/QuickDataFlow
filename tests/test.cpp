#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <gtest/gtest.h>

#include "shm_message.hpp"
#include "test.h"


void idle(std::atomic_bool *event_exit)
{
    std::string str;

    event_exit->store(true);
}

TEST(MessageBuff, output_message_waiter)
{
    using namespace fshm;
    using namespace std;
    char element1[1000];
    char element2[1000];
    memset(element1, 7, 1000);
    memset(element2, 8, 1000);

    MessageBuff msg_buff("shmin", "shmin", element1, element2, 100, 100, sizeof(element1), sizeof(element2));
    // unlock thread for output
    msg_buff.output_message_waiter.unlock();
    while(!msg_buff.output_message_complete.load());
    msg_buff.input_message_waiter.unlock();
    while(!msg_buff.input_message_complete.load());

    ASSERT_TRUE(0 == std::memcmp(element1, element2, sizeof(element1)));
}

TEST(MessageBuff, push_message_sync)
{
    using namespace fshm;
    using namespace std;
    char element1[1000];
    char element2[1000];
    memset(element1, 7, 1000);
    memset(element2, 8, 1000);

    MessageBuff msg_buff("shmin", "shmin", element1, element2, 100, 100, sizeof(element1), sizeof(element2));
    // unlock thread for output
    msg_buff.push_message_sync();
    msg_buff.get_message_sync();

    ASSERT_TRUE(0 == std::memcmp(element1, element2, sizeof(element1)));
}

/// spin node for push model
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
