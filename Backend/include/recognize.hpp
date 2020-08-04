#ifndef RECOGNIZE_HH
#define RECOGNIZE_HH

#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/face.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

void predict(const char *path_in, int &predictedLabel, double &confidence);

#endif