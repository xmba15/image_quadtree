/**
 * @file    Testimage_quadtree.cpp
 *
 * @author  btran
 *
 * @date    2019-08-28
 *
 * framework
 *
 * Copyright (c) organization
 *
 */

#include <gtest/gtest.h>

#include <image_quadtree/image_quadtree.hpp>

class Testimage_quadtree : public ::testing::Test
{
 protected:
    void SetUp() override
    {
        start_time_ = time(nullptr);
    }

    void TearDown() override
    {
        const time_t end_time = time(nullptr);

        // expect test time less than 10 sec
        EXPECT_LE(end_time - start_time_, 10);
    }

    time_t start_time_;
};

TEST_F(Testimage_quadtree, TestInitialization)
{
    ASSERT_TRUE(true);
}
