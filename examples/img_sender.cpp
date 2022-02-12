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
    MessageBuff msg_buff("img_sender_point_i_",
                         "img_sender_point_o_",
                         10,
                         10,
                         640 * 480 * 3,
                         640 * 480 * 3);

    cv::VideoCapture camera;
    camera.open(0);

    if (!camera.isOpened()) {
        cout << "not camera connect" << endl;
        return 0;
    }

    Mat inputImage;
    Mat sendImage(480, 640, CV_8UC3);
    namedWindow("camera image sender", WINDOW_NORMAL);

    ////////////////////// send video flow //////////////////////
    bool videoFlow{true};
    while (videoFlow) {
        if (!camera.read(inputImage)) {
            waitKey(100);
            continue;
        }

        resize(inputImage, sendImage, Size(640, 480));
        imshow("camera image sender", sendImage);

        uchar *arr = sendImage.isContinuous() ? sendImage.data: sendImage.clone().data;

        msg_buff.push_message_sync(static_cast<unsigned char*>(arr));

        // delay
        char key = static_cast<char>(waitKey(5));
        if(key == 'q' || key == 'Q')
        {
            videoFlow = false;
        }
    }
    return 0;
}
