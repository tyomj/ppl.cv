// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

#include "ppl/cv/x86/transpose.h"
#include "ppl/cv/x86/test.h"
#include <opencv2/imgproc.hpp>
#include <memory>
#include <gtest/gtest.h>
#include "ppl/cv/debug.h"
#include <iostream>

template <typename T, int nc>
void TransposeTest(int height, int width, T diff)
{
    std::unique_ptr<T[]> src(new T[width * height * nc]);
    std::unique_ptr<T[]> dst_ref(new T[width * height * nc]);
    std::unique_ptr<T[]> dst(new T[width * height * nc]);
    ppl::cv::debug::randomFill<T>(src.get(), width * height * nc, 0, 255);
    cv::Mat src_opencv(height, width, CV_MAKETYPE(cv::DataType<T>::depth, nc), src.get(), sizeof(T) * width * nc);
    cv::Mat dst_opencv(width, height, CV_MAKETYPE(cv::DataType<T>::depth, nc), dst_ref.get(), sizeof(T) * height * nc);

    cv::transpose(src_opencv, dst_opencv);
    ppl::cv::x86::Transpose<T, nc>(height, width, width * nc, src.get(), height * nc, dst.get());

    checkResult<T, nc>(dst_ref.get(), dst.get(), width, height, height * nc, height * nc, diff);
}

TEST(Transpose_FP32, x86)
{
    TransposeTest<float, 1>(720, 1080, 1.01f);
    TransposeTest<float, 3>(720, 1080, 1.01f);
    TransposeTest<float, 4>(720, 1080, 1.01f);
}

TEST(Transpose_UINT8, x86)
{
    TransposeTest<uint8_t, 1>(720, 1080, 1.01f);
    TransposeTest<uint8_t, 3>(720, 1080, 1.01f);
    TransposeTest<uint8_t, 4>(720, 1080, 1.01f);
}
