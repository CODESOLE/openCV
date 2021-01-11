#include <iostream>
#include <iterator>
#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/flann/logger.h>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv4/opencv2/opencv.hpp>

void demos()
{
    cv::Mat image_reference = cv::imread("reference_image.png");
    cv::Mat image_changed = cv::imread("reference_image.png");

    if (image_reference.empty())
    {
        std::cout << "Could not open or find the image 1" << std::endl;
        std::cin.get(); 
        return exit(EXIT_SUCCESS);
    }

    if (image_changed.empty())
    {
        std::cout << "Could not open or find the image 2" << std::endl;
        std::cin.get(); 
        return exit(EXIT_SUCCESS);
    }

    cv::namedWindow("Thresholded Image Reference", cv::WINDOW_NORMAL); 
    cv::namedWindow("Thresholded Image Changed", cv::WINDOW_NORMAL); 

    cv::namedWindow("Warning Window", cv::WINDOW_NORMAL);

    const int iLowH = 11;
    const int iHighH = 33;

    const int iLowS = 14;
    const int iHighS = 255;

    const int iLowV = 4;
    const int iHighV = 255;
#if 0
    // Create trackbars in "Control" window
    cv::createTrackbar("LowH", "Control", &iLowH, 179); // Hue (0 - 179)
    cv::createTrackbar("HighH", "Control", &iHighH, 179);

    cv::createTrackbar("LowS", "Control", &iLowS, 255); // Saturation (0 - 255)
    cv::createTrackbar("HighS", "Control", &iHighS, 255);

    cv::createTrackbar("LowV", "Control", &iLowV, 255); // Value (0 - 255)
    cv::createTrackbar("HighV", "Control", &iHighV, 255);
#endif
    while (true)
    {
        switch (std::getchar())
        {
        case 'x':
            image_changed = cv::imread("2_new4.png");
            break;
        case 'y':
            image_changed = cv::imread("2_new5.png");
            break;
        
        default:
            image_changed = cv::imread("2.png");
            break;
        }

        cv::Mat imgHSV_changed, imgHSV_reference;

        cv::cvtColor(image_reference, imgHSV_reference, cv::COLOR_BGR2HSV);                                 
        cv::cvtColor(image_changed, imgHSV_changed, cv::COLOR_BGR2HSV); 

        cv::Mat imgThresholded_changed, imgThresholded_reference;

        cv::inRange(imgHSV_reference, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded_reference); 

        // morphological opening (remove small objects from the foreground)
        cv::erode(imgThresholded_reference, imgThresholded_reference, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        cv::dilate(imgThresholded_reference, imgThresholded_reference, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

        // morphological closing (fill small holes in the foreground)
        cv::dilate(imgThresholded_reference, imgThresholded_reference, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        cv::erode(imgThresholded_reference, imgThresholded_reference, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        //----------------------------------------------------------------------------------------------------------------------------------------------
        cv::inRange(imgHSV_changed, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded_changed); // Threshold the image

        // morphological opening (remove small objects from the foreground)
        cv::erode(imgThresholded_changed, imgThresholded_changed, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        cv::dilate(imgThresholded_changed, imgThresholded_changed, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

        // morphological closing (fill small holes in the foreground)
        cv::dilate(imgThresholded_changed, imgThresholded_changed, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        cv::erode(imgThresholded_changed, imgThresholded_changed, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
        //----------------------------------------------------------------------------------------------------------------------------------------------
        cv::imshow("Thresholded Image Changed", imgThresholded_changed);          
        cv::imshow("Thresholded Image Reference", imgThresholded_reference); 

        cv::Mat img_result = image_changed;
        cv::cvtColor(img_result, img_result, cv::COLOR_BGR2GRAY); 

        for (int i = 0; i < imgThresholded_changed.rows; i++)
        {
            for (int j = 0; j < imgThresholded_changed.cols; j++)
            {
                if ((int)imgThresholded_changed.at<uint8_t>(i, j) == (int)imgThresholded_reference.at<uint8_t>(i, j))
                    img_result.at<uint8_t>(i, j) = 0;
                else
                    img_result.at<uint8_t>(i, j) = 255;
            }
        }
        // ---------------------------------------------- WARNING WINDOW ---------------------------------------------------

        bool isdifferent = false;

        for (int i = 0; i < imgThresholded_changed.rows; i++)
        {
            for (int j = 0; j < imgThresholded_changed.cols; j++)
            {
                if ((int)imgThresholded_changed.at<uint8_t>(i, j) != (int)imgThresholded_reference.at<uint8_t>(i, j))
                {
                    isdifferent = true;
                    goto warn_condtion;
                }
            }
        }
    warn_condtion:
        if (isdifferent)
        {
            cv::Mat warn_text(320, 240, CV_8UC3, cv::Scalar(0, 0, 0));
            cv::putText(warn_text, cv::String("WARNING!!!"), cv::Point2i(3,150), cv::FONT_HERSHEY_SIMPLEX, 1.4f, cv::Scalar(0, 0, 255));
            cv::putText(warn_text, cv::String("OBJECT DETECTED"), cv::Point2i(30,200), cv::FONT_HERSHEY_SIMPLEX, .5f, cv::Scalar(0, 0, 255));
            cv::putText(warn_text, cv::String("IN THE YELLOW LINE"), cv::Point2i(30,220), cv::FONT_HERSHEY_SIMPLEX, .5f, cv::Scalar(0, 0, 255));
            cv::imshow("Warning Window", warn_text);
            cv::imshow("Result Image", img_result);
        }
        else
        {
            cv::Mat warn_text(320, 240, CV_8UC3, cv::Scalar(0, 0, 0));
            cv::putText(warn_text, cv::String("NO OBJECT DETECTED"), cv::Point2i(30, 110), cv::FONT_HERSHEY_SIMPLEX, .5f, cv::Scalar(0, 255, 0));
            cv::putText(warn_text, cv::String("IN THE YELLOW LINE"), cv::Point2i(30, 130), cv::FONT_HERSHEY_SIMPLEX, .5f, cv::Scalar(0, 255, 0));
            cv::imshow("Warning Window", warn_text);
            cv::destroyWindow("Result Image");
        }


        if (cv::waitKey(30) == 27) 
        {
            std::cout << "esc key is pressed by user" << std::endl;
            break;
        }
    }

    cv::destroyWindow("Thresholded Image Reference"); 
    cv::destroyWindow("Thresholded Image Changed");
    cv::destroyWindow("Warning Window");
}



int main(int argc, char** argv)
{
    demos();
    return 0;
}