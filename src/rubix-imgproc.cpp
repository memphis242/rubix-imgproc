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

   // Let's now try some edge detectors, starting with Sobel
   // We've already got the grayscaled image, which is a typical first-step.
   // Now, we'll blur it, and then apply the 1D gradient operations, then
   // get the magnitude.
   cv::Mat img_gray_blurred;
   cv::GaussianBlur(img_gray, img_gray_blurred, cv::Size(3,3), 1.0);
   cv::Mat grad_x, grad_y;
   cv::Sobel(img_gray_blurred, grad_x, CV_32F, 1, 0, 3);
   cv::Sobel(img_gray_blurred, grad_y, CV_32F, 0, 1, 3);
   cv::Mat mag_grad;
   cv::magnitude(grad_x, grad_y, mag_grad);
   // Now that we've got the magnitude of the gradient, let's normalize and
   // threshold it before we display the image.
   cv::Mat img_edges_norm, img_edges_bin;
   cv::normalize(mag_grad, img_edges_norm, 0, 255, cv::NORM_MINMAX);
   cv::threshold(img_edges_norm, img_edges_bin, 50, 255, cv::THRESH_BINARY);

   cv::Mat img_edges_sobel = img_edges_bin;
   img_edges_sobel.convertTo(img_edges_sobel, CV_8U);
   cv::Mat img_edges_sobel_prethresh = img_edges_norm;
   img_edges_sobel_prethresh.convertTo(img_edges_sobel_prethresh, CV_8U);

   /***************************************************************************/

   // Display the image(s)
   cv::imshow("Mixed Rubik's Cube", img);
   //cv::imshow("Mixed Rubik's Cube (Grayscale)",         img_gray);
   //cv::imshow("Filtered Image (Vertical Kernel)",       img_filtered_vert);
   //cv::imshow("Filtered Image (Horizontal Kernel)",     img_filtered_horz);
   //cv::imshow("Gaussian Blurred Image (Built-In)",      img_gaussianblur_builtin);
   //cv::imshow("Gaussian Blurred Image (Manual Kernel)", img_gaussianblur_manualk);
   cv::imshow("Sobel Pre-Threshold",                    img_edges_sobel_prethresh);
   cv::imshow("Sobel'd Image",                          img_edges_sobel);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

