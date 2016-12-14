sn_drawbook SDK
    - Run on Nvidia Gforce Series / Telsa Series or CPU only
    - Fully Deep Learning Solution
    - 20 ms per 500 * 500 picture on Titan X using batch classify interface.

Support OS: ubuntu 14.04 / CentOS 7
Dependencies:
    - OpenCV 2.4.10
    - CUDA 7.5 [optional]
    - CUDNN v5 (7.5) [optional]
    - protobuf 2.5.0
    - glog 0.3.3
    - boost 1.46+
    - OpenBlas 0.2.15
Compile samples:
    - mkdir build && cd build
    - cmake -DCMAKE_INSTALL_PREFIX=../ ..
    - make && make install
    - cd ../bin
    - export LD_LIBRARY_PATH=../libs/ubuntu/cpu/
    - ./test_drawbook_recognition.bin images/test_83.jpg

Tutorials:
    - refer the library interface in the header file
    - refer the sample

Note: You need apply the sdk license key from SenseNets first.

Enjoy time!

Update Log:
	- v0.01 [2016.10.21]
		实现对90本绘本一共1252页面的识别
		在2497张测试图像上测试识别准确率为97.99%
	- v0.02 [2016.11.25]
		增加了页面识别置信度输出
		增加了图像特征的输出
		增加了通过检索进行识别
		增加了封面内页2分类识别，在2995张图像测试准确率为99.87%
		优化模型对封面的识别，在2995张图像测试准确率为98.03%
