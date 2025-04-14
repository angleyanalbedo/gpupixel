#include <iostream>
#include <gpupixel.h>
#include "gpupixel_wrapper.h"
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>




int main(){
    std::cout << "Starting GPU Pixel processing..." << std::endl;
    // Initialize GPU Pixel

    GpuPixelWrapper gpuPixel = GpuPixelWrapper();
    std::cout << "GPU Pixel wrapper initialized" << std::endl;

    // Load image
    std::cout << "Loading image..." << std::endl;
    gpuPixel.loadImage("demo.png");
    gpuPixel.initialize();
    cv::Mat image = cv::imread("demo.png", cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }
    cv::imshow("Original Image", image);
    cv::waitKey(0);
    std::cout << "Image loaded successfully" << std::endl;
    std::cout << "Image size: " << image.rows << "x" << image.cols << std::endl;
    gpuPixel.setParameters(5.0, 5.0, 5.0, 5.0, 5.0, 5.0);
    gpuPixel.processImage();
    std::shared_ptr<gpupixel::SinkRawData> sink_raw_data = gpuPixel.getSinkRawData();
    if (!sink_raw_data) {
        std::cerr << "Sink raw data is null" << std::endl;
        return -1;
    }
    int row = sink_raw_data->GetHeight();
    int col = sink_raw_data->GetWidth();
    const unsigned char* data = sink_raw_data->GetRgbaBuffer();
    cv::Mat result(cv::Size(row,col), CV_8UC4, (void*)data);
    std::cout << "Image processed successfully" << std::endl;
    std::cout << "Image size: " << row << "x" << col << std::endl;
    cv::imwrite("result.jpg", result);
    std::cout << "Image saved as result.jpg" << std::endl;
    cv::imshow("Result", result);
    cv::waitKey(0);
    return 0;
}