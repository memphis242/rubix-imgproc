#include <iostream>
//#include <ranges> FIXME: w64devkit has issues with >C++20
//#include <algorithm>

#include <opencv2/opencv.hpp>

int main(void)
{
   // Read in our image and make sure it loaded
   //cv::Mat img = cv::imread("mixed-rubiks-cube.jpg");
   cv::Mat img = cv::imread("lrg-mixed-rubiks-cube.jpg");
   if ( img.empty() )
   {
      std::cerr << "Could not load image." << std::endl;
      return 1;
   }
   cv::Size og_size = img.size();

   /**************************** Contour Detection ****************************/
   // By default, cv::imread returns a BGR color-coded cv::Mat object, so
   // convert to gray-scale.
   cv::Mat img_gray;
   cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

   /* Pre-process the Image */
   // Lower-resolution images will be faster to process and result in less
   // false-positives for contouring
   cv::Mat img_lowres;
   cv::resize(img_gray, img_lowres, cv::Size(160,128));
   // Apply a Gaussian blur to reduce noise and therefore odds of false-positives
   cv::Mat blurred;
   cv::GaussianBlur(img_gray, blurred, cv::Size(5,5), 1.0);

   /* Edge Detector */
   cv::Mat edges;
   cv::Canny(blurred, edges, 20, 50);

   /* Up-Scale Image */
   cv::Mat edges_upscaled;
   cv::resize(edges, edges_upscaled, og_size);

   /* Contour Detection */
   // Alrighty! Now let's have OpenCV find us the list of contours it can gather.
   std::vector<std::vector<cv::Point>> contours;
   cv::findContours(edges_upscaled, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
   // Filter smaller areas out
   std::vector<std::vector<cv::Point>> contours_filtered;
   for ( auto& contour : contours )
   {
      if ( cv::contourArea(contour) >= 0.25 )
         contours_filtered.push_back(contour);
   }

   // std::vector<std::vector<cv::Point>> contours_filtered = {
   //    contours |
   //    std::views::filter(
   //          [](const std::vector<cv::Point>& contour){
   //                cv::contourArea(contour) >= 100;
   //           } )
   // };

   /* Draw Contours */
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
   cv::imshow("Gray Blurred",                           blurred);
   cv::imshow("Canny Edges",                            edges);
   cv::imshow("Contour'd Image",                        img_contourd);
   cv::imshow("Contour'd Image (Filtered)",             img_contourd_filtered);

   // Print some stuff out
   std::cout << "contours.size(): " << contours.size() << '\n';
   std::cout << "contours_filtered.size(): " << contours_filtered.size() << '\n';

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

