#include <iostream>
#include <string>
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
   cv::imshow("Original Image", img);
   std::cout << "Original Size: " << img.rows << 'x' << img.cols << std::endl;
   cv::Size og_size = img.size();

   /**************************** Contour Detection ****************************/
   // By default, cv::imread returns a BGR color-coded cv::Mat object, so
   // convert to gray-scale.
   cv::Mat img_gray;
   cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);

   /* Pre-process the Image */
   // Lower-resolution images will be faster to process and result in less
   // false-positives for contouring
   //cv::Mat img_lowres;
   //cv::resize(img_gray, img_lowres, cv::Size(160,160));

   // Apply a Gaussian blur to reduce noise and therefore odds of false-positives
   //for ( size_t i = 7; i <= 35; i += 4 )
   //{
   //   cv::Mat blurred;
   //   std::string imstr = "";
   //   imstr += "Size: " + std::to_string(15) + ' ';
   //   imstr += "StdDev: " + std::to_string(static_cast<float>(i));
   //   cv::GaussianBlur( img_gray, blurred,
   //                     cv::Size(15,15),
   //                     static_cast<float>(i), static_cast<float>(i) );

   //   cv::imshow(imstr, blurred);
   //   cv::waitKey(0);
   //   cv::destroyWindow(imstr);
   //}
   cv::Mat blurred;
   // 15x15 is about 0.02% of 1024x1024
   cv::GaussianBlur(img_gray, blurred, cv::Size(15,15), 30.0f, 30.0f);
   cv::imshow("Blurred Image (15x15 Kernel, σ: 30.0x30.0)", blurred);
   cv::waitKey(0);

   ///* Edge Detector */
   //for ( size_t i = 15; i < 50; i += 5)
   //{
   //   cv::Mat edges;
   //   cv::Canny( blurred, edges,
   //              static_cast<float>(i) / 4.0f,
   //              static_cast<float>(i),
   //              3,
   //              true );
   //   std::string itrstr = "Canny: " + std::to_string(i);
   //   cv::imshow(itrstr, edges);
   //   cv::waitKey(0);
   //   cv::destroyWindow(itrstr);
   //}

   cv::Mat edges;
   cv::Canny( blurred, edges,
              25.0f / 4.0f,
              25.0f,
              3,
              true );
   cv::imshow("Canny", edges);

   cv::Mat closed;
   cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
   cv::morphologyEx(edges, closed, cv::MORPH_CLOSE, kernel);
   cv::imshow("Closed", closed);

   ///* Up-Scale Image */
   //cv::Mat edges_upscaled;
   //cv::resize(edges, edges_upscaled, og_size);

   /* Contour Detection */
   // Alrighty! Now let's have OpenCV find us the list of contours it can gather.
   std::vector<std::vector<cv::Point>> contours;
   cv::findContours(closed, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
   std::cout << contours.size() << std::endl;
   //cv::findContours(edges_upscaled, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
   // Filter smaller areas out
   //std::vector<std::vector<cv::Point>> contours_filtered;
   //for ( auto& contour : contours )
   //{
   //   if ( cv::contourArea(contour) >= 0.25 )
   //      contours_filtered.push_back(contour);
   //}

   //// std::vector<std::vector<cv::Point>> contours_filtered = {
   ////    contours |
   ////      std::views::filter(
   ////            [](const std::vector<cv::Point>& contour){
   ////                  cv::contourArea(contour) >= 100;
   ////             })
   //// };

   ///* Draw Contours */
   const cv::Scalar contour_color(240, 32, 160); // appprox teal
   //cv::Mat img_contoured = img.clone();
   //std::cout << "contours.size(): " << contours.size() << '\n';
   //for ( size_t i = 0; i < contours.size(); i++ )
   //{
   //   cv::drawContours(img_contoured, contours, i, contour_color);
   //   std::cout << cv::contourArea(contours[i]) << std::endl;
   //   cv::imshow("Contour'd Image", img_contoured);
   //   cv::waitKey(0);
   //}
   cv::Mat img_contoured = img.clone();
   cv::drawContours(img_contoured, contours, -1, contour_color);
   cv::imshow("Contour'd Image", img_contoured);
   //cv::drawContours(img_contoured_filtered, contours_filtered, -1, contour_color, 2);

   // Print some stuff out
   //std::cout << "contours_filtered.size(): " << contours_filtered.size() << '\n';


   /***************************************************************************/

   // Display the image(s)
   //cv::imshow("Mixed Rubik's Cube", img);
   //cv::imshow("Mixed Rubik's Cube (Grayscale)",         img_gray);
   //cv::imshow("Filtered Image (Vertical Kernel)",       img_filtered_vert);
   //cv::imshow("Filtered Image (Horizontal Kernel)",     img_filtered_horz);
   //cv::imshow("Gaussian Blurred Image (Built-In)",      img_gaussianblur_builtin);
   //cv::imshow("Gaussian Blurred Image (Manual Kernel)", img_gaussianblur_manualk);
   //cv::imshow("Sobel Pre-Threshold",                    img_edges_sobel_prethresh);
   //cv::imshow("Sobel'd Image",                          img_edges_sobel);
   //cv::imshow("Gray Blurred",                           blurred);
   //cv::imshow("Canny Edges",                            edges);
   //cv::imshow("Contour'd Image",                        img_contoured);
   //cv::imshow("Contour'd Image (Filtered)",             img_contoured_filtered);

   // Wait indefinitely until a key is pressed
   cv::waitKey(0);

   // Manually destroy all windows just to clean up
   cv::destroyAllWindows();

   return 0;
}

