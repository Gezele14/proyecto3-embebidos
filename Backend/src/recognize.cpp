
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/face.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
static void read_file_in(const std::string &filename, std::vector<cv::Mat> &images)
{
    std::ifstream file(filename.c_str(), std::ifstream::in);
    if (!file)
    {
        std::string error_message = "No valid input file was given, please check the given filename.";
        std::cerr << error_message << std::endl;
        exit(1);
    }
    std::string line, path;
    while (getline(file, line))
    {
        std::stringstream liness(line);
        getline(liness, path);
        if (!path.empty())
        {
            std::cout<<"path: " << path << "\n";
            images.push_back(cv::imread(path));
        }
    }
}

cv::Mat getFace(cv::Mat image_In)
{
    if(image_In.empty()){
        std::cerr<<"vacia\n";
        exit(1);
    }
    
    cv::Mat image_gray, image_gray_crop;

    cv::CascadeClassifier face_cascade;
    cv::CascadeClassifier eyes_cascade;
    cv::CascadeClassifier smile_cascade;

    face_cascade.load("haarcascade_frontalface_alt.xml");
    // Detect faces
    std::vector<cv::Rect> faces;
    
    cv::cvtColor(image_In, image_gray, cv::COLOR_BGR2GRAY);

    //cv::equalizeHist(image_gray, image_gray);
    // cv::normalize(image_gray, image_gray, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    face_cascade.detectMultiScale(image_gray, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(80, 80));
    std::cout<<"e\n";
    image_gray_crop = image_gray(faces[0]);
    resize(image_gray_crop, image_gray_crop, {92, 112}, 2, 2);
    return image_gray_crop;
}

int main(int argc, char const *argv[])
{
    
    cv::Mat img_in;
    const char* path_in = argv[1];

    std::cout<<"path img in: "<< path_in<<std::endl;
    img_in=cv::imread(path_in);
    img_in=getFace(img_in);
    //cv::namedWindow("im", cv::WINDOW_NORMAL);
    //cv::imshow("im",img_in);

    cv::Ptr<cv::face::FaceRecognizer> model = cv::face::FisherFaceRecognizer::create();
    model->read("trained_model");
    int predictedLabel = -1;
    double confidence = 0.0;
    model->predict(img_in, predictedLabel, confidence);

    std::string result_message = cv::format("Predicted class = %d / confidence = %f", predictedLabel,confidence);
    std::cout << result_message << std::endl;


//cv::waitKey(0);
}