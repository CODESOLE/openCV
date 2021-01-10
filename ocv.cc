#include <cstdlib>
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
        cv::Mat image_cpy = cv::imread("2_new4.png");
        cv::Mat image = cv::imread("2.png");

        // Check for failure
        if (image.empty())
        {
            std::cout << "Could not open or find the image" << std::endl;
            std::cin.get(); // wait for any key press
            return exit(EXIT_SUCCESS);
        }

        cv::namedWindow("Thresholded Image", cv::WINDOW_NORMAL); // show the thresholded image
        cv::namedWindow("Thresholded Image Copy", cv::WINDOW_NORMAL); // show the original image

        int iLowH = 11;
        int iHighH = 33;

        int iLowS = 14;
        int iHighS = 255;

        int iLowV = 4;
        int iHighV = 255;
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
            cv::Mat imgHSV, imgHSV_cpy;

            cv::cvtColor(image_cpy, imgHSV_cpy,
                cv::COLOR_BGR2HSV); // Convert the captured frame
                                    // from BGR to HSV

            cv::cvtColor(image, imgHSV, cv::COLOR_BGR2HSV); // Convert the captured frame from BGR to HSV

            cv::Mat imgThresholded, imgThresholded_cpy;

            cv::inRange(imgHSV_cpy, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV),
                imgThresholded_cpy); // Threshold the image

            // morphological opening (remove small objects from the foreground)
            cv::erode(imgThresholded_cpy, imgThresholded_cpy,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            cv::dilate(imgThresholded_cpy, imgThresholded_cpy,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            // morphological closing (fill small holes in the foreground)
            cv::dilate(imgThresholded_cpy, imgThresholded_cpy,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            cv::erode(imgThresholded_cpy, imgThresholded_cpy,
                cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            //----------------------------------------------------------------------------------------------------------------------------------------------
            cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV),
                imgThresholded); // Threshold the image

            // morphological opening (remove small objects from the foreground)
            cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            // morphological closing (fill small holes in the foreground)
            cv::dilate(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));

            cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)));
            //----------------------------------------------------------------------------------------------------------------------------------------------
            cv::imshow("Thresholded Image", imgThresholded); // show the thresholded image
            cv::imshow("Thresholded Image Copy", imgThresholded_cpy); // show the original image

            cv::Mat img_result = image;
            cv::cvtColor(img_result, img_result, cv::COLOR_BGR2GRAY); // Convert the captured frame from BGR to HSV


             for (int i = 0; i < imgThresholded.rows; i++)
             {
                 for (int j = 0; j < imgThresholded.cols; j++)
                 {
                     if ((int)imgThresholded.at<uint8_t>(i, j) == (int)imgThresholded_cpy.at<uint8_t>(i, j))
                         img_result.at<uint8_t>(i, j) = 0;
                     else
                         img_result.at<uint8_t>(i, j) = 255;
                 }
             }
             cv::imshow("result image", img_result);



            if (cv::waitKey(30) == 27) // wait for 'esc' key press for 30ms.
            {
                std::cout << "esc key is pressed by user" << std::endl;
                break;
            }
        }
    //EXIT:
        cv::destroyWindow("Thresholded Image Copy"); // destroy the created window
        cv::destroyWindow("Thresholded Image");      // destroy the created 
    }

    


int main(int argc, char** argv)
{
    demos();
    return 0;
}
