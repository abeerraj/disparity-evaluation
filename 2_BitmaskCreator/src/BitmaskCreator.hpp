#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Frame.hpp"
#include "Bitmask.hpp"

class BitmaskCreator {
  public:
    static Bitmask getOccludedPixels(Frame frame, float threshold = 2.0f);
    static Bitmask getTexturedPixels(Frame frame, int width = 3, float threshold = 4.0f);
    static Bitmask getDepthDiscontinuedPixels(Frame frame, float gap = 2.0f, int width = 9);
};
