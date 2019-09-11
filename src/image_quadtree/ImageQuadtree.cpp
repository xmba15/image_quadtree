/**
 * @file    ImageQuadtree.cpp
 *
 * @author  btran
 *
 * @date    2019-08-30
 *
 * Copyright (c) organization
 *
 */

#include <opencv2/opencv.hpp>

#include "image_quadtree/ImageQuadtree.hpp"
#include "image_quadtree/Utility.hpp"

namespace pcv
{
ImageQuadtree::ImageQuadtree() : _root(nullptr)
{
}

ImageQuadtree::ImageQuadtree(const cv::Mat& img, const size_t minLeafSize, const double maxVariant)
    : _imgName(""), _minLeafSize(minLeafSize), _maxVariant(maxVariant)
{
    assert(!img.empty() && img.isContinuous());

    this->_data = new uchar[img.total() * img.elemSize()];

    this->_imgCols = img.cols;
    this->_imgRows = img.rows;
    this->_imgChannel = img.channels();
    memcpy(this->_data, img.data, img.total() * img.elemSize() * sizeof(uchar));

    this->_root = this->createImageQuadrant(0, this->_imgRows - 1, 0, this->_imgCols - 1, 0);
}

ImageQuadtree::ImageQuadtree(const std::string& imgName, const size_t minLeafSize, const double maxVariant)
    : _imgName(imgName), _minLeafSize(minLeafSize), _maxVariant(maxVariant)
{
    cv::Mat img = cv::imread(imgName);
    assert(!img.empty() && img.isContinuous());

    // reread image as a 3-channel image if the current image has 4 channels
    if (img.channels() > 3) {
        img = cv::imread(imgName, 1);
    }

    this->_data = new uchar[img.total() * img.elemSize()];
    this->_imgCols = img.cols;
    this->_imgRows = img.rows;
    this->_imgChannel = img.channels();
    memcpy(this->_data, img.data, img.total() * img.elemSize() * sizeof(uchar));

    this->_root = this->createImageQuadrant(0, this->_imgRows - 1, 0, this->_imgCols - 1, 0);

    img.release();
}

ImageQuadtree::~ImageQuadtree()
{
    delete _root;
    // delete[] _data;
}

ImageQuadtree::ImageQuadrant* ImageQuadtree::createImageQuadrant(const uint32_t rMin, const uint32_t rMax,
                                                                 const uint32_t cMin, const uint32_t cMax,
                                                                 uint8_t level)
{
    assert(rMin <= rMax && cMin <= cMax);

    ImageQuadrant::Ptr imageQuadrant = new ImageQuadrant;
    imageQuadrant->isLeaf = true;
    imageQuadrant->level = level;
    imageQuadrant->rMin = rMin;
    imageQuadrant->rMax = rMax;
    imageQuadrant->cMin = cMin;
    imageQuadrant->cMax = cMax;
    imageQuadrant->level = level;

    static const std::vector<float> MAX_VARIANT(this->_imgChannel, this->_maxVariant);
    const std::vector<float> varPixel = this->estimateVariancePixelValue(imageQuadrant);

    bool moreThanMaxVariant = false;
    for (size_t i = 0; i < this->_imgChannel; ++i) {
        if (varPixel[i] > MAX_VARIANT[i]) {
            moreThanMaxVariant = true;
            break;
        }
    }

    if ((rMax - rMin) <= this->_minLeafSize || (cMax - cMin) <= this->_minLeafSize || !moreThanMaxVariant) {
        const std::vector<float> averagePixel = this->estimateAveragePixelValue(imageQuadrant);

        imageQuadrant->pixel.reserve(this->_imgChannel);
        std::transform(averagePixel.begin(), averagePixel.end(), std::back_inserter(imageQuadrant->pixel),
                       [](const float& elem) { return static_cast<uchar>(elem); });
        return imageQuadrant;
    }

    // split the current quadtree
    imageQuadrant->isLeaf = false;
    /*
              0  1  2  3
       row    -  +  -  +
       col    -  -  +  +
    */

    const uint32_t rMid = (rMin + rMax) * 0.5f;
    const uint32_t cMid = (cMin + cMax) * 0.5f;

    imageQuadrant->child[0] = this->createImageQuadrant(rMin, rMid, cMin, cMid, level + 1);
    imageQuadrant->child[1] = this->createImageQuadrant(rMid + 1, rMax, cMin, cMid, level + 1);
    imageQuadrant->child[2] = this->createImageQuadrant(rMin, rMid, cMid + 1, cMax, level + 1);
    imageQuadrant->child[3] = this->createImageQuadrant(rMid + 1, rMax, cMid + 1, cMax, level + 1);

    return imageQuadrant;
}

const std::vector<float> ImageQuadtree::estimateAveragePixelValue(const ImageQuadrant* quadrant) const
{
    std::vector<float> result(this->_imgChannel, 0.0);

    for (uint32_t ir = quadrant->rMin; ir <= quadrant->rMax; ++ir) {
        for (uint32_t ic = quadrant->cMin; ic <= quadrant->cMax; ++ic) {
            for (size_t k = 0; k < this->_imgChannel; ++k) {
                result[k] += this->_data[ir * this->_imgCols * this->_imgChannel + ic * this->_imgChannel + k];
            }
        }
    }

    for (float& elem : result) {
        elem /= quadrant->numPixels();
    }

    return result;
}

const std::vector<float> ImageQuadtree::estimateVariancePixelValue(const ImageQuadrant* quadrant) const
{
    const std::vector<float> averagePixel = this->estimateAveragePixelValue(quadrant);

    std::vector<float> result(this->_imgChannel, 0.0);
    for (uint32_t ir = quadrant->rMin; ir <= quadrant->rMax; ++ir) {
        for (uint32_t ic = quadrant->cMin; ic <= quadrant->cMax; ++ic) {
            for (size_t k = 0; k < this->_imgChannel; ++k) {
                const uchar val = this->_data[ir * this->_imgCols * this->_imgChannel + ic * this->_imgChannel + k];
                result[k] += std::pow((averagePixel[k] - val), 2);
            }
        }
    }

    for (float& elem : result) {
        elem /= quadrant->numPixels();
    }

    return result;
}

uchar* ImageQuadtree::unpackQuadtree() const
{
    uchar* unpackedData =
        reinterpret_cast<uchar*>(malloc(this->_imgRows * this->_imgCols * this->_imgChannel * sizeof(uchar)));
    this->unpackQuadtree(this->_root, unpackedData);

    return unpackedData;
}

cv::Mat ImageQuadtree::unpackcvMatQuadtree() const
{
    switch (this->_imgChannel) {
        case 1: {
            return cv::Mat(this->_imgRows, this->_imgCols, CV_8UC1, this->unpackQuadtree());
            break;
        }
        case 3: {
            return cv::Mat(this->_imgRows, this->_imgCols, CV_8UC3, this->unpackQuadtree());
            break;
        }
        default:
            throw std::runtime_error("image must be of 1 or 3 channels");
            break;
    }
}

cv::Mat ImageQuadtree::unpackcvMatArtQuadtree(const ART_MODE mode) const
{
    cv::Mat result;

    switch (this->_imgChannel) {
        case 1: {
            result = cv::Mat(this->_imgRows, this->_imgCols, CV_8UC1);
            break;
        }
        case 3: {
            result = cv::Mat(this->_imgRows, this->_imgCols, CV_8UC3);
            break;
        }
        default:
            throw std::runtime_error("image must be of 1 or 3 channels");
            break;
    }

    std::vector<ImageQuadrant::Ptr> imageQuadrants;
    imageQuadrants.emplace_back(this->_root);

    while (!imageQuadrants.empty()) {
        const ImageQuadrant::Ptr curImageQuadrantPtr = imageQuadrants.back();
        imageQuadrants.pop_back();

        if (!curImageQuadrantPtr->isLeaf) {
            if (curImageQuadrantPtr->child[0] != nullptr) {
                for (size_t i = 0; i < 4; ++i) {
                    imageQuadrants.emplace_back(curImageQuadrantPtr->child[i]);
                }
            }

            continue;
        }

        cv::Scalar color;
        if (this->_imgChannel == 1) {
            color = cv::Scalar(curImageQuadrantPtr->pixel.front());
        }
        if (this->_imgChannel == 3) {
            color =
                cv::Scalar(curImageQuadrantPtr->pixel[0], curImageQuadrantPtr->pixel[1], curImageQuadrantPtr->pixel[2]);
        }

        switch (mode) {
            case ART_MODE::RECTANGLE: {
                cv::rectangle(result, cv::Point(curImageQuadrantPtr->cMin, curImageQuadrantPtr->rMin),
                              cv::Point(curImageQuadrantPtr->cMax, curImageQuadrantPtr->rMax), color, -1);
                break;
            }
            case ART_MODE::ELLIPSE: {
                cv::RotatedRect rect(cv::Point(curImageQuadrantPtr->cMin, curImageQuadrantPtr->rMin),
                                     cv::Point(curImageQuadrantPtr->cMin, curImageQuadrantPtr->rMax),
                                     cv::Point(curImageQuadrantPtr->cMax, curImageQuadrantPtr->rMax));

                cv::ellipse(result, rect, color, -1);
                break;
            }
            case ART_MODE::HEART_SHAPE: {
                pcv::drawHeartShape(result, cv::Point(curImageQuadrantPtr->cMin, curImageQuadrantPtr->rMin),
                                    cv::Point(curImageQuadrantPtr->cMax, curImageQuadrantPtr->rMax), color);
                break;
            }
            default:
                break;
        }
    }

    return result;
}

void ImageQuadtree::unpackQuadtree(const ImageQuadrant* quadrant, uchar* unpackedData) const
{
    if (quadrant->isLeaf) {
        for (uint32_t ir = quadrant->rMin; ir <= quadrant->rMax; ++ir) {
            for (uint32_t ic = quadrant->cMin; ic <= quadrant->cMax; ++ic) {
                for (size_t k = 0; k < this->_imgChannel; ++k) {
                    unpackedData[ir * this->_imgCols * this->_imgChannel + ic * this->_imgChannel + k] =
                        quadrant->pixel[k];
                }
            }
        }
        return;
    }

    for (size_t i = 0; i < 4; ++i) {
        this->unpackQuadtree(quadrant->child[i], unpackedData);
    }
}

ImageQuadtree::ImageQuadrant::ImageQuadrant() : isLeaf(true), rMin(0), rMax(0), cMin(0), cMax(0)
{
    memset(&child, 0, 4 * sizeof(ImageQuadrant::Ptr));
}

ImageQuadtree::ImageQuadrant::~ImageQuadrant()
{
    for (uint32_t i = 0; i < 4; ++i) {
        delete child[i];
    }
}

const uint32_t ImageQuadtree::ImageQuadrant::numPixels() const
{
    return (rMax - rMin + 1) * (cMax - cMin + 1);
}

}  // namespace pcv
