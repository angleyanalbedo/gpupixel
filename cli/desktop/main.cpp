#include <iostream>
#include <gpupixel.h>
#include "gpupixel_wrapper.h"
#include <memory>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>


#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

GpuPixelWrapper gpuPixel = GpuPixelWrapper();

int processImage(cv::Mat& image,parameters& param){
    // Load image
    std::cout << "Loading image..." << std::endl;
    if(image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }
    gpuPixel.loadImage(image.data, image.cols, image.rows, image.channels());
    gpuPixel.addFilter();
    
    std::cout << "Image loaded successfully" << std::endl;
    std::cout << "Image size: " << image.rows << "x" << image.cols << std::endl;
    gpuPixel.setParameters(param);
    gpuPixel.processImage();
    std::shared_ptr<gpupixel::SinkRawData> sink_raw_data = gpuPixel.getSinkRawData();
    if (!sink_raw_data) {
        std::cerr << "Sink raw data is null" << std::endl;
        return -1;
    }
    int row = sink_raw_data->GetHeight();
    int col = sink_raw_data->GetWidth();
    const unsigned char* data = sink_raw_data->GetRgbaBuffer();
    cv::Mat result(cv::Size(col,row), CV_8UC4, (void*)data);
    // cv::cvtColor(result, result, cv::COLOR_RGBA2BGR);
    image = result;
    return 0;

}

int testProcrssVideo(){
    std::cout << "Loading video..." << std::endl;
    cv::VideoCapture cap();

    if (!cap.isOpened()) {
        std::cerr << "Could not open or find the video!" << std::endl;
        return -1;
    }


    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break; // End of video
        }
        cv::imshow("Original Video", frame);

        processImage(frame, parameters{0,0,0,0,0,0});
        cv::imshow("Processed Video", frame);
        if (cv::waitKey(30) ==27) {
            break; // Exit on key press
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}

int processVideo(std::string& path){
    // Load video
    std::cout << "Loading video..." << std::endl;
    cv::VideoCapture cap(path);

    if (!cap.isOpened()) {
        std::cerr << "Could not open or find the video!" << std::endl;
        return -1;
    }

    parameters param{0,0,0,0,0,0};
    int beautyValue = 0;
    int whithValue = 0;
    int thinFaceValue = 0;
    int bigeyeValue = 0;
    int lipstickValue = 0;
    int blusherValue = 0;
    cv::namedWindow("Processed Video", cv::WINDOW_NORMAL);
    cv::createTrackbar("BeautyValue", "Processed Video", &beautyValue, 10);
    cv::createTrackbar("WhithValue", "Processed Video", &whithValue, 10);
    cv::createTrackbar("ThinFaceValue", "Processed Video", &thinFaceValue, 10);
    cv::createTrackbar("BigeyeValue", "Processed Video", &bigeyeValue, 10);
    cv::createTrackbar("LipstickValue", "Processed Video", &lipstickValue, 10);
    cv::createTrackbar("BlusherValue", "Processed Video", &blusherValue, 10);

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break; // End of video
        }
        cv::imshow("Original Video", frame);

        processImage(frame, parameters{(float)beautyValue, (float)whithValue,(float)thinFaceValue,(float) bigeyeValue,(float)lipstickValue, (float) blusherValue});
        cv::imshow("Processed Video", frame);
        if (cv::waitKey(30) ==27) {
            break; // Exit on key press
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}


int testImage(std::string& path,parameters& param,std::string& output_path){
    // Load image
    std::cout << "Loading image..." << std::endl;
    cv::Mat image = cv::imread(path, cv::IMREAD_UNCHANGED);

    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    cv::imshow("Original Image", image);
    cv::waitKey(10);
    std::cout << "Image loaded successfully" << std::endl;
    std::cout << "Image size: " << image.rows << "x" << image.cols << std::endl;
    processImage(image, param);
    cv::imwrite(output_path, image);
    std::cout << "Image saved as "<<output_path << std::endl;
    cv::imshow("Result", image);
    cv::waitKey(0);
    return 0;
}

int main(){
    gpuPixel.initialize();
    // testImage(std::string("demo.png"),parameters{0,0,0,0,0,0}, std::string("output.png"));
    
    testProcrssVideo();
}