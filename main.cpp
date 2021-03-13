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


void idle(std::atomic_bool *event_exit)
{
    std::string str;

    event_exit->store(true);
}

TEST(MessageBuffTest, MessageBuffSendRecv){
    using namespace fshm;
    using namespace std;
    char element1[1000];
    char element2[1000];
    memset(element1, 7, 1000);
    memset(element2, 8, 1000);

    MessageBuff msg_buff("shmin3", "shmin3", element1, element2, 100, 100, sizeof(element1), sizeof(element2));
    // unlock thread for output
    msg_buff.output_message_waiter.unlock();
    // check complete
    cout << msg_buff.output_message_complete.load() << endl;
    while(!msg_buff.output_message_complete.load())
    {
        cout << "wait output" << endl;
    }
    cout << msg_buff.output_message_complete.load() << endl;

    msg_buff.input_message_waiter.unlock();
    cout << msg_buff.input_message_complete.load() << endl;
    while(!msg_buff.input_message_complete.load())
    {
        cout << "wait input" << endl;
    }
    cout << msg_buff.input_message_complete.load() << endl;
    cout << endl;

    EXPECT_TRUE(0 == std::memcmp(element1, element2, sizeof(element1)));

}


/// spin node for push model
int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
