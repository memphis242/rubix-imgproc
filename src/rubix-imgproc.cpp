#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char * argv[])
{
   // Read in our image and make sure it loaded
   cv::Mat img = cv::imread("lrg-mixed-rubiks-cube.jpg");
   if ( img.empty() )
   {
      std::cerr << "Could not load image." << std::endl;
      return 1;
   }

   /**************************** Process the Image ****************************/
   // By default, cv::imread returns a BGR color-coded cv::Mat object
   cv::Mat img_gray;
   cv::cvtColor( img, img_gray, cv::COLOR_BGR2GRAY );

   /***************************************************************************/

   // Display the image(s)
   cv::namedWindow("Mixed Rubik's Cube", cv::WINDOW_AUTOSIZE);
   cv::imshow("Mixed Rubik's Cube", img);
   cv::namedWindow("Mixed Rubik's Cube (Grayscale)", cv::WINDOW_AUTOSIZE);
   cv::imshow("Mixed Rubik's Cube (Grayscale)", img_gray);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

