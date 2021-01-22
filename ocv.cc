#include <cstdlib>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>

void
run ()
{
        cv::Mat image = cv::imread ("/home/caner/Pictures/nature2.png");

        if (image.empty ())
          {
            std::cout << "Could not open or find the image" << std::endl;
            std::cin.get (); 
            return exit (EXIT_SUCCESS);
          }

        cv::namedWindow ("Control", cv::WINDOW_AUTOSIZE);

        int iLowH = 0;
        int iHighH = 179;

        int iLowS = 0;
        int iHighS = 255;

        int iLowV = 0;
        int iHighV = 255;

        cv::createTrackbar ("LowH", "Control", &iLowH, 179); 
        cv::createTrackbar ("HighH", "Control", &iHighH, 179);

        cv::createTrackbar ("LowS", "Control", &iLowS, 255);
        cv::createTrackbar ("HighS", "Control", &iHighS, 255);

        cv::createTrackbar ("LowV", "Control", &iLowV, 255);
        cv::createTrackbar ("HighV", "Control", &iHighV, 255);

        while (true)
          {
            cv::Mat imgHSV;

            cv::cvtColor (image, imgHSV, cv::COLOR_BGR2HSV); 

            cv::Mat imgThresholded;

            cv::inRange (imgHSV, cv::Scalar (iLowH, iLowS, iLowV), cv::Scalar (iHighH, iHighS, iHighV), imgThresholded);

            cv::erode (imgThresholded, imgThresholded, cv::getStructuringElement (cv::MORPH_ELLIPSE, cv::Size (5, 5)));

            cv::dilate (imgThresholded, imgThresholded, cv::getStructuringElement (cv::MORPH_ELLIPSE, cv::Size (5, 5)));

            cv::dilate (imgThresholded, imgThresholded, cv::getStructuringElement (cv::MORPH_ELLIPSE, cv::Size (5, 5)));

            cv::erode (imgThresholded, imgThresholded, cv::getStructuringElement (cv::MORPH_ELLIPSE, cv::Size (5, 5)));

            cv::imshow ("Thresholded Image", imgThresholded);
            cv::imshow ("Original", image);

            if (cv::waitKey (30) == 27)
              {
                std::cout << "esc key is pressed by user" << std::endl;
                break;
              }
          }
        cv::destroyWindow ("Control"); 
        cv::destroyWindow ("Original");
        cv::destroyWindow ("Thresholded Image");
}

int
main (int argc, char **argv)
{
  run ();
  return 0;
}