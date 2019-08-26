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

#ifndef uchar
#define uchar unsigned char
#endif  // uchar

class ImageQuadtree
{
 public:
    ImageQuadtree();

    ~ImageQuadtree();

 private:
    struct ImageQuadrant {
        ImageQuadrant();

        ~ImageQuadrant();

        bool isLeaf;

        ImageQuadrant* child[4];
    };

 private:
    size_t HEIGHT;

    size_t WIDTH;

    uchar* data;

    ImageQuadrant* root;
};
