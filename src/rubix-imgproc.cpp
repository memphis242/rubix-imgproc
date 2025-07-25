#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char * argv[])
{
   cv::Mat img = cv::imread("mixed-rubiks-cube.jpg");

   if ( img.empty() )
   {
      std::cerr << "Could not load image." << std::endl;
      return 1;
   }

   cv::namedWindow("Mixed Rubik's Cube", cv::WINDOW_AUTOSIZE);
   cv::imshow("Mixed Rubik's Cube", img);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

