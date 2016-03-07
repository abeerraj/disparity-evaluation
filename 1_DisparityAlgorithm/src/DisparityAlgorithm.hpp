#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class DisparityAlgorithm {
  public:
    virtual void compute() = 0;
    virtual ~DisparityAlgorithm() {};

    Mat& getResult() {
      return result;
    }

    void setResult(Mat result) {
      this->result = result;
    }

    Mat getNormalizedResult() {
      return result;
      int rows = result.size().height;
      int cols = result.size().width;
      Mat normalized = Mat::zeros(rows, cols, CV_8U);

      uchar max = 63;
      uchar newMax = 255;

      for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
          uchar value = result.at<uchar>(y, x);
          normalized.at<uchar>(y,x) = value * newMax / max;
        }
      }

      // TODO define min/max?
      // TODO camera setup?

      //Mat normalized;
      //normalize(result, normalized, 0, 255, CV_MINMAX, CV_8U);
      return normalized;
    }

  private:
    Mat result;
};
