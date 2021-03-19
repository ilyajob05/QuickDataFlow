#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <atomic>
#include <stdio.h>
#include <string>
#include <iostream>
#include <cstring>
#include <array>
#include "shm_message.hpp"


void idle(std::atomic_bool *event_exit)
{
    std::string str;

    event_exit->store(true);
}


/// spin node for push model
int main(int argc, char* argv[]) {
    using namespace fshm;
    using namespace std;
    std::array<unsigned char, 1000> element1;
    std::array<unsigned char, 1000> element2;
    memset(element1.data(), 7, 1000);
    memset(element2.data(), 8, 1000);

    // shmin - name of shared memory
    // element1 memory source
    // element2 memory destination
    // 100 queue elements num of input
    // 100 queue elements num of output
    MessageBuff msg_buff("shmin", "shmin", static_cast<unsigned char*> (element1.data()), static_cast<unsigned char*> (element2.data()), 100, 100, sizeof(element1), sizeof(element2));
    // unlock thread for write to output
    msg_buff.output_message_waiter.unlock();
    // check complete
    cout << msg_buff.output_message_complete.load() << endl;

    msg_buff.push_message_sync();

    cout << msg_buff.output_message_complete.load() << endl;

    msg_buff.input_message_waiter.unlock();
    cout << msg_buff.input_message_complete.load() << endl;

    msg_buff.get_message_sync();

    cout << msg_buff.input_message_complete.load() << endl;
    cout << endl;

    if(0 != std::memcmp(static_cast<unsigned char*> (element1.data()), element2.data(), sizeof(element1))){
        cout << "error" << endl;
        return -1;
    }
    return 0;
}

