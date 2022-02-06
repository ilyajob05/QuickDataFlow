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


int main(int argc, char *argv[])
{
    MessageBuff msg_buff("img_sender_point_o",
                         "img_sender_point_i",
                         10,
                         10,
                         640 * 480 * 3,
                         640 * 480 * 3);

//    Mat inputImage(480, 640, CV_8UC3);
    namedWindow("camera image receiver", WINDOW_NORMAL);
    ////////////////////// send video flow //////////////////////
    bool videoFlow = true;
    uchar img_buff[640 * 480 * 3];
    uchar img_second_buff[640 * 480 * 3];
    uchar *sw_buff = img_buff;
    uchar *sw_old_buff = img_second_buff;

//    while (videoFlow) {
//        //        msg_buff.get_message_sync(msg_buff.buff.data());
//        sw_buff = (sw_buff == img_buff) ? img_second_buff : img_buff;
////        memset(sw_buff, 0, 640 * 480 * 3);
//        msg_buff.get_message_sync(sw_buff);

////        inputImage.data = img_buff;
//        Mat inputImage(480, 640, CV_8UC3, sw_buff);

//        imshow("camera image receiver", inputImage);
//        // delay
//        char key = static_cast<char>(waitKey(10));
//        if (key == 'q' || key == 'Q') {
//            videoFlow = false;
//        }
//        cout << '.' << endl;

//        sw_old_buff = sw_buff;
//    }

    int counter{0};
    while (videoFlow) {
        //        memset(sw_buff, 0, 640 * 480 * 3);
        msg_buff.get_message_sync(sw_buff);

        Mat inputImage(480, 640, CV_8UC3, sw_buff);

        imshow("camera image receiver", inputImage);
        cout << counter++ << endl;

        // delay
        char key = static_cast<char>(waitKey(5));
        if (key == 'q' || key == 'Q') {
            videoFlow = false;
        }
        cout << '.' << endl;

        sw_old_buff = sw_buff;
    }


    return 0;
}


