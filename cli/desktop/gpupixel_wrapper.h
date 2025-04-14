#pragma once
#include <gpupixel.h>
#include <memory>
#include <opencv2/core.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define DEBUG 1
#define USE_GLFW 0

class GpuPixelWrapper
{
private:

    std::shared_ptr<gpupixel::SourceImage> source_image;
    std::shared_ptr<gpupixel::SinkRawData> sink_raw_data;
    std::shared_ptr<gpupixel::BeautyFaceFilter> beauty_face_filter;
    std::shared_ptr<gpupixel::FaceReshapeFilter> face_reshape_filter;
    std::shared_ptr<gpupixel::LipstickFilter> lipstick_filter;
    std::shared_ptr<gpupixel::BlusherFilter> blusher_filter;
    std::shared_ptr<gpupixel::FaceDetector> face_detector;
    GLFWwindow* window; // OpenGL context
    
    /* data */
    

public:
    GpuPixelWrapper() : source_image(nullptr), sink_raw_data(nullptr), beauty_face_filter(nullptr), face_reshape_filter(nullptr), lipstick_filter(nullptr), blusher_filter(nullptr), face_detector(nullptr) {
        #ifdef DEBUG
        std::cout << "GpuPixelWrapper constructor called" << std::endl;
        #endif
        
    };
    ~GpuPixelWrapper(){}

    #if USE_GLFW
    bool 
    #else
    void
    #endif
    
    initialize(){
    #if USE_GLFW
        // initialize OpenGL functions
        window = gpupixel::GPUPixelContext::GetInstance()->GetGLContext();
        if (window == nullptr) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            return false;
        }
        glfwMakeContextCurrent(window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            return false;
        }
    #endif
        #ifdef DEBUG
        std::cout << "Initializing GPU Pixel..." << std::endl;
        #endif
        sink_raw_data = gpupixel::SinkRawData::Create();
        beauty_face_filter = gpupixel::BeautyFaceFilter::Create();
        face_reshape_filter = gpupixel::FaceReshapeFilter::Create();
        lipstick_filter = gpupixel::LipstickFilter::Create();
        blusher_filter = gpupixel::BlusherFilter::Create();
        face_detector = gpupixel::FaceDetector::Create();
        #ifdef DEBUG
        std::cout << "GPU Pixel initialized" << std::endl;
        #endif
        #ifdef DEBUG
        std::cout << "Creating filters..." << std::endl;
        #endif
        source_image->AddSink(beauty_face_filter)
            ->AddSink(face_reshape_filter)
            ->AddSink(lipstick_filter)
            ->AddSink(blusher_filter)
            ->AddSink(sink_raw_data);
        #ifdef DEBUG
        std::cout << "Filters created" << std::endl;
        #endif
        
    }
    void loadImage(const std::string& image_path){
        source_image = gpupixel::SourceImage::Create(image_path);
    }
    void loadImage(const cv::Mat& image){
        if (image.empty()) {
            throw std::runtime_error("Image is empty");
        }
        const unsigned char* data = reinterpret_cast<const unsigned char*>(image.data);
        source_image = gpupixel::SourceImage::CreateFromBuffer( image.cols, image.rows,image.channels(), data);
    }
    void loadImage(const unsigned char* data, int width, int height, int channels){
        source_image = gpupixel::SourceImage::CreateFromBuffer(width, height, channels, data);
    }
    void setBeautyFaceFilter(float value){
        if (!beauty_face_filter) {
            beauty_face_filter = gpupixel::BeautyFaceFilter::Create();
        }
        beauty_face_filter->SetBlurAlpha(value);
    }
    void processImage();
    void setParameters(float beautyValue, float whithValue, float thinFaceValue, float bigeyeValue, float lipstickValue, float blusherValue);
    std::shared_ptr<gpupixel::SourceImage> getSourceImage() const {
        return source_image;
    }
    std::shared_ptr<gpupixel::SinkRawData> getSinkRawData() const {
        return sink_raw_data;
    }
};
void GpuPixelWrapper::setParameters(float beautyValue, float whithValue, float thinFaceValue, float bigeyeValue, float lipstickValue, float blusherValue) {
    // check parameters
    if (beautyValue < 0.0) beautyValue = 0.0;
    if (beautyValue > 10.0) beautyValue = 10.0;
    if (whithValue < 0.0) whithValue = 0.0;
    if (whithValue > 10.0) whithValue = 10.0;
    if (thinFaceValue < 0.0) thinFaceValue = 0.0;
    if (thinFaceValue > 10.0) thinFaceValue = 10.0;
    if (bigeyeValue < 0.0) bigeyeValue = 0.0;
    if (bigeyeValue > 10.0) bigeyeValue = 10.0;
    if (lipstickValue < 0.0) lipstickValue = 0.0;
    if (lipstickValue > 10.0) lipstickValue = 10.0;
    if (blusherValue < 0.0) blusherValue = 0.0;
    if (blusherValue > 10.0) blusherValue = 10.0;

    // set parameters for filters
    beauty_face_filter->SetBlurAlpha(beautyValue / 10);
    beauty_face_filter->SetWhite(whithValue / 20);
    face_reshape_filter->SetFaceSlimLevel(thinFaceValue / 10);
    face_reshape_filter->SetEyeZoomLevel(bigeyeValue / 10);
    lipstick_filter->SetBlendLevel(lipstickValue / 10);
    blusher_filter->SetBlendLevel(blusherValue / 10);
}
void GpuPixelWrapper::processImage(){
    if (!source_image) {
        throw std::runtime_error("Source image is not loaded");
    }
    if (!sink_raw_data) {
        throw std::runtime_error("Sink raw data is not initialized");
    }
    if (!beauty_face_filter) {
        throw std::runtime_error("Beauty face filter is not initialized");
    }
    if (!face_reshape_filter) {
        throw std::runtime_error("Face reshape filter is not initialized");
    }
    if (!lipstick_filter) {
        throw std::runtime_error("Lipstick filter is not initialized");
    }
    if (!blusher_filter) {
        throw std::runtime_error("Blusher filter is not initialized");
    }
    if (!face_detector) {
        throw std::runtime_error("Face detector is not initialized");
    }

    int width = source_image->GetWidth();
    int height = source_image->GetHeight();
    const unsigned char* buffer = source_image->GetRgbaImageBuffer();

  // Detect face landmarks
    std::vector<float> landmarks = face_detector->Detect(
      buffer, width, height, width * 4, gpupixel::GPUPIXEL_MODE_FMT_PICTURE,
      gpupixel::GPUPIXEL_FRAME_TYPE_RGBA);

    if (!landmarks.empty()) {
      lipstick_filter->SetFaceLandmarks(landmarks);
      blusher_filter->SetFaceLandmarks(landmarks);
      face_reshape_filter->SetFaceLandmarks(landmarks);
    }
    // Process the image with the filters
    source_image->Render();
}