#pragma once

#include <opencv2/opencv.hpp>

cv::Mat open_file(std::string image_path);
cv::Mat gray_scale(cv::Mat img);
cv::Mat binary(cv::Mat img, int amount=80);
std::vector<std::vector<int>> prepare_matrix_img(std::string image_path, int height);
std::vector<std::vector<int>> prepare_matrix_ter(std::string image_path, int height);
