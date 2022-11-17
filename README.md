# QuickDataFlow

[![Build Status](https://travis-ci.com/ilyajob05/QuickDataFlow.svg?branch=main)](https://travis-ci.com/ilyajob05/QuickDataFlow)
[![codecov](https://codecov.io/gh/ilyajob05/QuickDataFlow/branch/main/graph/badge.svg?token=T2M14V6SK8)](https://codecov.io/gh/ilyajob05/QuickDataFlow)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=ilyajob05_QickDataFlow&metric=alert_status)](https://sonarcloud.io/dashboard?id=ilyajob05_QickDataFlow)
[![CMake](https://github.com/ilyajob05/QuickDataFlow/actions/workflows/cmake.yml/badge.svg)](https://github.com/ilyajob05/QuickDataFlow/actions/workflows/cmake.yml)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/ilyajob05/QuickDataFlow/main/LICENSE)
![PyPI](https://img.shields.io/pypi/v/QuickDataFlow?label=pypi%20QuickDataFlow)
<!--- ![PyPI - Downloads](https://img.shields.io/pypi/dm/QuickDataFlow)
--->

This library is for building multithreaded and fast C++ applications

## Requirements
- gtest
- wheel

## Install
### Build for C++
``` commandline
git clone https://github.com/ilyajob05/QuickDataFlow.git
cd QuickDataFlow
chmod +x build.sh
./build.sh
```
### Install from deb
Your need build library before install
``` commandline
sudo pdkg -i build/QuickDataFlow-2.1.0-Linux.deb
```

### Install from Pypi
``` commandline
pip install QuickDataFlow
```
or download file from [PyPi](https://pypi.org/project/QuickDataFlow/#files) whl or tar.gz
``` commandline
pip install QuickDataFlow-2.1.2.tar.gz 
# or
# pip install QuickDataFlow-2.1.2-cp310-cp310-manylinux2014_x86_64.whl 
```

## Usage
### C++
```cpp
#include <opencv2/opencv.hpp>
#include "QuickDataFlow/shm_message.hpp"

using namespace fshm;
using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    // create a buffer for data transfer
    MessageBuff msg_buff("img_sender_point_i_", "img_sender_point_o_",
                         10, 10, 640 * 480 * 3, 640 * 480 * 3);

    Mat inputImage = imread("data/test-image-1.png");
    Mat sendImage(480, 640, CV_8UC3);
    resize(inputImage, sendImage, Size(640, 480));
    namedWindow("camera image sender", WINDOW_NORMAL);
    imshow("camera image sender", sendImage);
    
    // send data
    unsigned char *arr = sendImage.isContinuous() ? sendImage.data: sendImage.clone().data;
    msg_buff.push_message_sync(static_cast<unsigned char*>(arr));

    return 0;
}

```
### Python
```python
from PIL import Image
from QuickDataFlow import shm_message as sm

mb_buff = sm.MessageBuff("img_sender_point_o_", "img_sender_point_i_", 10, 10, 640 * 480 * 3, 640 * 480 * 3)
buff = bytes([0] * (640 * 480 * 3))
mb_buff.get_msg(buff)
img = Image.frombytes("RGB", (640,480), buff)
img.show()
```


![](./output.png)
