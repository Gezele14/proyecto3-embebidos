
#include <recognize.hpp>

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

void predict(const char* path_in, int &predictedLabel, double &confidence)
{
    
    cv::Mat img_in;

    std::cout<<"path img in: "<< path_in<<std::endl;
    img_in=cv::imread(path_in);
    img_in=getFace(img_in);
    //cv::namedWindow("im", cv::WINDOW_NORMAL);
    //cv::imshow("im",img_in);

    cv::Ptr<cv::face::FaceRecognizer> model = cv::face::FisherFaceRecognizer::create();
    model->read("trained_model");
    predictedLabel = -1;
    confidence = 0.0;
    model->predict(img_in, predictedLabel, confidence);

    std::string result_message = cv::format("Predicted class = %d / confidence = %f", predictedLabel,confidence);
    std::cout << result_message << std::endl;
}