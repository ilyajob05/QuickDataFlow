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
#include <array>

#include "shm_message.hpp"
#include "test.hpp"


void idle(std::atomic_bool *event_exit)
{
    std::string str;

    event_exit->store(true);
}

TEST(MessageBuff, output_message_waiter)
{
    using namespace fshm;
    using namespace std;
    std::array<unsigned char, 1000> element1;
    std::array<unsigned char, 1000> element2;
    memset(element1.data(), 7, 1000);
    memset(element2.data(), 8, 1000);

    MessageBuff msg_buff("shmin", "shmin", static_cast<unsigned char*> (element1.data()), static_cast<unsigned char*> (element2.data()), 100, 100, sizeof(element1), sizeof(element2));
    // unlock thread for output
    msg_buff.output_message_waiter.unlock();
    while(!msg_buff.output_message_complete.load());
    msg_buff.input_message_waiter.unlock();
    while(!msg_buff.input_message_complete.load());

    ASSERT_TRUE(0 == std::memcmp(element1.data(), element2.data(), sizeof(element1)));
}

TEST(MessageBuff, push_message_sync)
{
    using namespace fshm;
    using namespace std;
    std::array<unsigned char, 1000> element1;
    std::array<unsigned char, 1000> element2;
    memset(element1.data(), 7, 1000);
    memset(element2.data(), 8, 1000);

    MessageBuff msg_buff("shmin", "shmin", static_cast<unsigned char*> (element1.data()), static_cast<unsigned char*> (element2.data()), 100, 100, element1.size(), element2.size());
    // unlock thread for output
    msg_buff.push_message_sync();
    msg_buff.get_message_sync();

    ASSERT_TRUE(0 == std::memcmp(static_cast<unsigned char*> (element1.data()), static_cast<unsigned char*> (element2.data()), element1.size()));
}

/// spin node for push model
int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
