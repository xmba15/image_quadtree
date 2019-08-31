/**
 * @file    ImageQuadtree.hpp
 *
 * @author  btran
 *
 * @date    2019-08-27
 *
 * Copyright (c) organization
 *
 */

#pragma once

#include <cstdlib>
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#ifndef uchar
typedef unsigned char uchar;
#endif  // uchar

namespace pcv
{
class ImageQuadtree
{
 public:
    enum ART_MODE {
        RECTANGLE = 0,
        ELLIPSE = 1,
        HEART_SHAPE = 2,
    };

    struct ImageQuadrant {
        using Ptr = ImageQuadrant*;

        ImageQuadrant();

        ~ImageQuadrant();

        bool isLeaf;
        uint8_t level;

        uint32_t rMin;
        uint32_t rMax;
        uint32_t cMin;
        uint32_t cMax;

        std::vector<uchar> pixel;

        const uint32_t numPixels() const;

        ImageQuadrant* child[4];
    };

    ImageQuadtree();

    explicit ImageQuadtree(const cv::Mat& img, const size_t leafSize = 4, const double _maxVariant = 50);

    explicit ImageQuadtree(const std::string& imgName, const size_t leafSize = 4, const double _maxVariant = 50);

    ~ImageQuadtree();

    uchar* unpackQuadtree() const;

    cv::Mat unpackcvMatQuadtree() const;

    cv::Mat unpackcvMatArtQuadtree(const ART_MODE mode = ART_MODE::RECTANGLE) const;

    void unpackcvMatArtQuadtree(cv::Mat& result, const ART_MODE mode = ART_MODE::RECTANGLE) const;

    const size_t imgCols() const
    {
        return this->_imgCols;
    }

    const size_t imgRows() const
    {
        return this->_imgRows;
    }

    const size_t imgChannel() const
    {
        return this->_imgChannel;
    }

 private:
    ImageQuadrant::Ptr createImageQuadrant(const uint32_t rMin, const uint32_t rMax, const uint32_t cMin,
                                           const uint32_t cMax, uint8_t level);

    const std::vector<float> estimateAveragePixelValue(const ImageQuadrant* quadrant) const;
    const std::vector<float> estimateVariancePixelValue(const ImageQuadrant* quadrant) const;

    void unpackQuadtree(const ImageQuadrant* quadrant, uchar* unpackedData) const;

 private:
    std::string _imgName;

    size_t _imgCols;

    size_t _imgRows;

    size_t _imgChannel;

    cv::Mat _img;

    size_t _minLeafSize;

    ImageQuadrant::Ptr _root;

    double _maxVariant;
};
}  // namespace pcv
