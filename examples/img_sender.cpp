#include <opencv2/opencv.hpp>
#include "shm_message.hpp"

using namespace fshm;
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // create a buffer for data transfer
    MessageBuff msg_buff("img_sender_point_i_", "img_sender_point_o_",
                         10, 10, 640 * 480 * 3, 640 * 480 * 3);
    Mat inputImage = imread("../data/test-image-1.png");
    Mat sendImage(480, 640, CV_8UC3);
    resize(inputImage, sendImage, Size(640, 480));
    cvtColor(sendImage, sendImage, COLOR_BGR2RGB); // for PIL
    namedWindow("camera image sender", WINDOW_NORMAL);
    // send data
    uchar* arr = sendImage.isContinuous() ? sendImage.data : sendImage.clone().data;
    msg_buff.push_message_sync(static_cast<unsigned char*>(arr));
    imshow("camera image sender", inputImage);
    waitKey(10000);
    return 0;
}
