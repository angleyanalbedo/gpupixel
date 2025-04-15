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

int processVideo(std::string& path){
    std::cout << "Starting GPU Pixel processing..." << std::endl;
    // Initialize GPU Pixel

    GpuPixelWrapper gpuPixel = GpuPixelWrapper();
    std::cout << "GPU Pixel wrapper initialized" << std::endl;

    // Load video
    std::cout << "Loading video..." << std::endl;
    cv::VideoCapture cap(path);

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
        gpuPixel.setParameters(0, 0, 0, 0, 0, 0);
        // gpuPixel.processImage(frame);
        std::shared_ptr<gpupixel::SinkRawData> sink_raw_data = gpuPixel.getSinkRawData();
        if (!sink_raw_data) {
            std::cerr << "Sink raw data is null" << std::endl;
            return -1;
        }
        int row = sink_raw_data->GetHeight();
        int col = sink_raw_data->GetWidth();
        const unsigned char* data = sink_raw_data->GetRgbaBuffer();
        cv::Mat result(cv::Size(col,row), CV_8UC4, (void*)data);
        cv::cvtColor(result, result, cv::COLOR_RGBA2BGR);
        cv::imshow("Processed Video", result);
        if (cv::waitKey(30) >= 0) {
            break; // Exit on key press
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}


int testImage(std::string& path,parameters& param){
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
    cv::imwrite("result.jpg", image);
    std::cout << "Image saved as result.jpg" << std::endl;
    cv::imshow("Result", image);
    cv::waitKey(0);
    return 0;
}

int main(){
    gpuPixel.initialize();
    testImage(std::string("demo.png"),parameters{0,0,0,0,0,0});
}