#pragma once

#include "RubixCommonTypes.hpp"
#include "opencv2/opencv.hpp"

/* Forward Declarations */
namespace Rubix
{
   class ImgProc;
}

class Rubix::ImgProc
{
   public:
      ImgProc( /* img */ );
      ~ImgProc();

      void compute_mx(void);
      Rubix::CubeMx cubemx(void);
      Rubix::FaceMx facemx(Face);

   private:
      // TODO: private membersimg, matrix
      cv::Mat img;
      Rubix::CubeMx cube_mx;
}
