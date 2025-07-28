#include <iostream>
#include <opencv2/opencv.hpp>

int main(void)
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

   // Let's play with different kernels to convolve /w the image
   float gain = 0.333333f;
   cv::Mat k_vert = (cv::Mat_<float>(3,3) <<
         0, 1, 0,
         0, 1, 0,
         0, 1, 0 );
   cv::Mat img_filtered_vert;
   cv::filter2D(img_gray, img_filtered_vert, -1, k_vert);

   cv::Mat k_horz = (cv::Mat_<float>(3,3) <<
         0, 0, 0,
         1, 1, 1,
         0, 0, 0 );
   cv::Mat img_filtered_horz;
   cv::filter2D(img_gray, img_filtered_horz, -1, k_horz);

   // Let's try some Gaussian blurs
   double sigma = 10.0;
   // First, the built-in opencv function cv::GaussianBlur()
   cv::Mat img_gaussianblur_builtin;
   cv::GaussianBlur(img, img_gaussianblur_builtin, cv::Size(15,15), sigma);
   // Now, try first building the kernel using the other built-in kernel generating fcns
   cv::Mat k1d = cv::getGaussianKernel(15, sigma, CV_64F);
   cv::Mat k2d = k1d * k1d.t();
   cv::Mat img_gaussianblur_manualk;
   cv::filter2D(img, img_gaussianblur_manualk, -1, k2d);

   /***************************************************************************/

   // Display the image(s)
   cv::imshow("Mixed Rubik's Cube", img);
   cv::imshow("Mixed Rubik's Cube (Grayscale)",         img_gray);
   cv::imshow("Filtered Image (Vertical Kernel)",       img_filtered_vert);
   cv::imshow("Filtered Image (Horizontal Kernel)",     img_filtered_horz);
   cv::imshow("Gaussian Blurred Image (Built-In)",      img_gaussianblur_builtin);
   cv::imshow("Gaussian Blurred Image (Manual Kernel)", img_gaussianblur_manualk);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

