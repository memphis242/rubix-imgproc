#include <iostream>
//#include <ranges> FIXME: w64devkit has issues with >C++20
//#include <algorithm>

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

   //// Let's play with different kernels to convolve /w the image
   //cv::Mat k_vert = (cv::Mat_<float>(3,3) <<
   //      0, 1, 0,
   //      0, 1, 0,
   //      0, 1, 0 );
   //cv::Mat img_filtered_vert;
   //cv::filter2D(img_gray, img_filtered_vert, -1, k_vert);

   //cv::Mat k_horz = (cv::Mat_<float>(3,3) <<
   //      0, 0, 0,
   //      1, 1, 1,
   //      0, 0, 0 );
   //cv::Mat img_filtered_horz;
   //cv::filter2D(img_gray, img_filtered_horz, -1, k_horz);

   //// Let's try some Gaussian blurs
   //double sigma = 10.0;
   //// First, the built-in opencv function cv::GaussianBlur()
   //cv::Mat img_gaussianblur_builtin;
   //cv::GaussianBlur(img, img_gaussianblur_builtin, cv::Size(15,15), sigma);
   //// Now, try first building the kernel using the other built-in kernel generating fcns
   //cv::Mat k1d = cv::getGaussianKernel(15, sigma, CV_64F);
   //cv::Mat k2d = k1d * k1d.t();
   //cv::Mat img_gaussianblur_manualk;
   //cv::filter2D(img, img_gaussianblur_manualk, -1, k2d);

   // Let's now try some edge detectors, starting with Sobel
   // We've already got the grayscaled image, which is a typical first-step.
   // Now, we'll blur it, and then apply the 1D gradient operations, then
   // get the magnitude.
   //cv::Mat img_gray_blurred, edges, binary;
   //// Now that we've got the magnitude of the gradient, let's normalize and
   //// threshold it before we display the image.
   //cv::Mat img_edges_norm, img_edges_bin;
   //cv::normalize(mag_grad, img_edges_norm, 0, 255, cv::NORM_MINMAX);
   //cv::threshold(img_edges_norm, img_edges_bin, 50, 255, cv::THRESH_BINARY);

   //cv::Mat img_edges_sobel = img_edges_bin;
   //img_edges_sobel.convertTo(img_edges_sobel, CV_8U);
   //cv::Mat img_edges_sobel_prethresh = img_edges_norm;
   //img_edges_sobel_prethresh.convertTo(img_edges_sobel_prethresh, CV_8U);

   // Let's try Canny out
   cv::Mat img_gray_blurred, canny_edges, canny_binary;
   cv::GaussianBlur(img_gray, img_gray_blurred, cv::Size(5,5), 1.0);
   cv::Canny(img_gray_blurred, canny_edges, 20, 50);

   // Alrighty! Now let's have OpenCV find us the list of contours it can gather.
   cv::Mat edges = canny_edges;
   std::vector<std::vector<cv::Point>> contours;
   cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
   // Filter smaller areas out
   std::vector<std::vector<cv::Point>> contours_filtered;
   for ( auto& contour : contours )
   {
      if ( cv::contourArea(contour) >= 0.5 )
         contours_filtered.push_back(contour);
   }
   // std::vector<std::vector<cv::Point>> contours_filtered = {
   //    contours |
   //    std::views::filter(
   //          [](const std::vector<cv::Point>& contour){
   //                cv::contourArea(contour) >= 100;
   //           } )
   // };

   // Let's draw the contours found on the original image
   const cv::Scalar contour_color(240, 32, 160); // appprox teal
   cv::Mat img_contourd = img.clone();
   cv::Mat img_contourd_filtered = img.clone();
   cv::drawContours(img_contourd, contours, -1, contour_color, 2);
   cv::drawContours(img_contourd_filtered, contours_filtered, -1, contour_color, 2);

   /***************************************************************************/

   // Display the image(s)
   cv::imshow("Mixed Rubik's Cube", img);
   //cv::imshow("Mixed Rubik's Cube (Grayscale)",         img_gray);
   //cv::imshow("Filtered Image (Vertical Kernel)",       img_filtered_vert);
   //cv::imshow("Filtered Image (Horizontal Kernel)",     img_filtered_horz);
   //cv::imshow("Gaussian Blurred Image (Built-In)",      img_gaussianblur_builtin);
   //cv::imshow("Gaussian Blurred Image (Manual Kernel)", img_gaussianblur_manualk);
   //cv::imshow("Sobel Pre-Threshold",                    img_edges_sobel_prethresh);
   //cv::imshow("Sobel'd Image",                          img_edges_sobel);
   cv::imshow("Gray Blurred",                           img_gray_blurred);
   cv::imshow("Canny Edges",                            canny_edges);
   cv::imshow("Contour'd Image",                        img_contourd);
   cv::imshow("Contour'd Image (Filtered)",             img_contourd_filtered);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

