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
#include <opencv2/opencv.hpp>
#include "shm_message.hpp"


using namespace fshm;
using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
    MessageBuff msg_buff("img_sender_point_o_",
                         "img_sender_point_i_",
                         10,
                         10,
                         640 * 480 * 3,
                         640 * 480 * 3);
    //    Mat inputImage(480, 640, CV_8UC3);
    namedWindow("camera image receiver", WINDOW_NORMAL);
    ////////////////////// send video flow //////////////////////
    bool videoFlow = true;
    uchar img_buff[640 * 480 * 3];

    while (videoFlow) {
        // wait message in internal cycle
        msg_buff.get_message_sync(img_buff);
        Mat inputImage(480, 640, CV_8UC3, img_buff);
        imshow("camera image receiver", inputImage);
        // delay
        char key = static_cast<char>(waitKey(1));

        if (key == 'q' || key == 'Q') {
            videoFlow = false;
        }
    }

    return 0;
}


