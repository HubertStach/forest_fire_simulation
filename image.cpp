#include "image.h"
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

cv::Mat open_file(std::string image_path){
    try{
        cv::Mat img =cv::imread(image_path, cv::IMREAD_COLOR);
        if(img.empty()){
            std::cout << "Could not open or find the image: " << image_path << std::endl;
            return img;
        }
        return img;
    }
    catch( cv::Exception& e){
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
    }
}

cv::Mat gray_scale(cv::Mat img){
    try{
        cv::Mat new_img = cv::Mat::zeros(img.size(), img.type());

        if(img.empty()){
            std::cout << "Could not open or find the image: " << std::endl;
            return img;
        }

        int rows = img.rows;
        int cols = img.cols;

        // konwertowanie do szarości poprzez uśrednienie wartości rgb
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols;j++){
                cv::Vec3b bgr_pixel = img.at<cv::Vec3b>(i,j);
                
                int blue = bgr_pixel[0];
                int green = bgr_pixel[1];
                int red = bgr_pixel[2];

                int gray = (blue + green + red)/3;

                new_img.at<cv::Vec3b>(i,j) = cv::Vec3b(gray, gray, gray);
            }
        }
        
        img = new_img;

        return img;
    }catch( cv::Exception& e){
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
        return img;
    }
}

cv::Mat binary(cv::Mat img, int amount){
    try{
        cv::Mat new_img = cv::Mat::zeros(img.size(), img.type());

        if(img.empty()){
            std::cout << "Could not open or find the image: " << std::endl;
            return img;
        }

        int rows = img.rows;
        int cols = img.cols;

        float percentage = 255*(amount/100.0f);

        // do każdego pixela dodajemy tę samą wartość
        for(int i=0; i<rows; i++){
            for(int j=0; j<cols;j++){
                cv::Vec3b bgr_pixel = img.at<cv::Vec3b>(i,j);

                int blue = bgr_pixel[0];
                int green = bgr_pixel[1];
                int red = bgr_pixel[2];

                int new_color = (blue + green + red)/3;


                if(new_color > percentage){new_color = 0;}
                else{new_color = 255;}

                new_img.at<cv::Vec3b>(i,j) = cv::Vec3b(new_color, new_color, new_color);
            }
        }
        
        img = new_img;

        return img;
    }catch( cv::Exception& e){
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
        return img;
    }
}

std::vector<std::vector<int>> prepare_matrix(std::string image_path, int height){

    try{
        cv::Mat image = open_file(image_path);

        //skala szarosci
        image = gray_scale(image);

        //binaryzacja
        image = binary(image);

        // Transpozycja obrazu (zamiana wierszy z kolumnami)
        cv::Mat transposed_image;
        cv::transpose(image, transposed_image);

        // Skalowanie do wysokości 200 z zachowaniem proporcji
        int target_height = height;
        int original_height = transposed_image.rows;
        int original_width = transposed_image.cols;
        
        // Oblicz nową szerokość zachowując proporcje
        double scale_factor = static_cast<double>(target_height) / original_height;
        int target_width = static_cast<int>(original_width * scale_factor);
        
        // Przeskaluj obraz
        cv::Mat resized_image;
        cv::resize(transposed_image, resized_image, cv::Size(target_width, target_height), 0, 0, cv::INTER_LINEAR);

        int rows = resized_image.rows;
        int cols = resized_image.cols;       
        
        std::vector<std::vector<int>> result_mat(rows, std::vector<int> (cols));

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                cv::Vec3b bgr_pixel = resized_image.at<cv::Vec3b>(i,j);
                int value = bgr_pixel[0];

                if(value == 0){value = 0;}
                else{value = 3;}

                result_mat[i][j] = value;
            }
        }

        return result_mat;

    }catch(cv::Exception& e){
        const char* err_msg = e.what();
        std::cout << "exception caught: " << err_msg << std::endl;
        std::vector<std::vector<int>> result_mat(3, std::vector<int> (3));
        return result_mat;
    }
}