#include <cstdlib>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv4/opencv2/opencv.hpp>

void
demos ()
{
        cv::Mat image = cv::imread ("/home/caner/Pictures/nature2.png");

        // Check for failure
        if (image.empty ())
          {
            std::cout << "Could not open or find the image" << std::endl;
            std::cin.get (); // wait for any key press
            return exit (EXIT_SUCCESS);
          }

        cv::namedWindow (
            "Control",
            cv::WINDOW_AUTOSIZE); // create a window called "Control"

        int iLowH = 0;
        int iHighH = 179;

        int iLowS = 0;
        int iHighS = 255;

        int iLowV = 0;
        int iHighV = 255;

        // Create trackbars in "Control" window
        cv::createTrackbar ("LowH", "Control", &iLowH, 179); // Hue (0 - 179)
        cv::createTrackbar ("HighH", "Control", &iHighH, 179);

        cv::createTrackbar ("LowS", "Control", &iLowS,
                            255); // Saturation (0 - 255)
        cv::createTrackbar ("HighS", "Control", &iHighS, 255);

        cv::createTrackbar ("LowV", "Control", &iLowV, 255); // Value (0 - 255)
        cv::createTrackbar ("HighV", "Control", &iHighV, 255);

        while (true)
          {
            cv::Mat imgHSV;

            cv::cvtColor (image, imgHSV,
                          cv::COLOR_BGR2HSV); // Convert the captured frame
                                              // from BGR to HSV

            cv::Mat imgThresholded;

            cv::inRange (imgHSV, cv::Scalar (iLowH, iLowS, iLowV),
                         cv::Scalar (iHighH, iHighS, iHighV),
                         imgThresholded); // Threshold the image

            // morphological opening (remove small objects from the foreground)
            cv::erode (imgThresholded, imgThresholded,
                       cv::getStructuringElement (cv::MORPH_ELLIPSE,
                                                  cv::Size (5, 5)));

            cv::dilate (imgThresholded, imgThresholded,
                        cv::getStructuringElement (cv::MORPH_ELLIPSE,
                                                   cv::Size (5, 5)));

            // morphological closing (fill small holes in the foreground)
            cv::dilate (imgThresholded, imgThresholded,
                        cv::getStructuringElement (cv::MORPH_ELLIPSE,
                                                   cv::Size (5, 5)));

            cv::erode (imgThresholded, imgThresholded,
                       cv::getStructuringElement (cv::MORPH_ELLIPSE,
                                                  cv::Size (5, 5)));

            cv::imshow ("Thresholded Image",
                        imgThresholded);          // show the thresholded image
            cv::imshow ("Original", image); // show the original image

            if (cv::waitKey (30) == 27) // wait for 'esc' key press for 30ms.
                                        // If 'esc' key is pressed, break loop
              {
                std::cout << "esc key is pressed by user" << std::endl;
                break;
              }
          }
        cv::destroyWindow ("Control"); // destroy the created window
        cv::destroyWindow ("Original"); // destroy the created window
        cv::destroyWindow ("Thresholded Image"); // destroy the created window
}

int
main (int argc, char **argv)
{
  demos ();
  return 0;
}