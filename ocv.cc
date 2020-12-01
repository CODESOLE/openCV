#include <cstdlib>
#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv4/opencv2/opencv.hpp>

void
demos (size_t demo_id = 0)
{
  switch (demo_id)
    {
    case 0:
      {
        // Read the image file
        cv::Mat image = cv::imread ("/home/caner/Pictures/boostNote.png");

        // Check for failure
        if (image.empty ())
          {
            std::cout << "Could not open or find the image" << std::endl;
            std::cin.get (); // wait for any key press
            return exit(EXIT_SUCCESS);
          }

        cv::String windowName = "OpenCV"; // Name of the window

        cv::namedWindow (windowName, cv::WINDOW_NORMAL); // Create a window

        cv::imshow (windowName, image); // Show our image inside the created window.

        cv::waitKey (0); // Wait for any keystroke in the window

        cv::destroyWindow (windowName); // destroy the created window

        break;
      }
    case 1:
      {
        // open the video file for reading
        cv::VideoCapture cap ("/home/caner/Misc/non_lin/nonlinear_week2.mp4");

        // if not success, exit program
        if (cap.isOpened () == false)
          {
            std::cout << "Cannot open the video file" << std::endl;
            std::cin.get (); // wait for any key press
            return exit(EXIT_FAILURE);
          }

        // Uncomment the following line if you want to start the video in the
        // middle cap.set(CAP_PROP_POS_MSEC, 300);

        // get the frames rate of the video
        double fps = cap.get (cv::CAP_PROP_FPS);
        std::cout << "Frames per seconds : " << fps << std::endl;

        cv::String window_name = "My First Video";

        cv::namedWindow (window_name, cv::WINDOW_NORMAL); // create a window

        while (true)
          {
            cv::Mat frame;
            bool bSuccess = cap.read (frame); // read a new frame from video

            // Breaking the while loop at the end of the video
            if (bSuccess == false)
              {
                std::cout << "Found the end of the video" << std::endl;
                break;
              }

            // show the frame in the created window
            cv::imshow (window_name, frame);

            // wait for for 10 ms until any key is pressed.
            // If the 'Esc' key is pressed, break the while loop.
            // If the any other key is pressed, continue the loop
            // If any key is not pressed withing 10 ms, continue the loop
            if (cv::waitKey (10) == 27)
              {
                std::cout << "Esc key is pressed by user. Stoppig the video"
                     << std::endl;
                break;
              }
          }
        break;
      }
    case 2:{

             break;
           }
    }
}

int
main (int argc, char **argv)
{
  demos(1);
  return 0;
}
