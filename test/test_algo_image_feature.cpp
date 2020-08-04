///
/// \brief Unit tests for image feature algorithms.
/// \author alex011235
/// \date 2020-06-28
/// \link <a href=https://github.com/alex011235/algo>Algo, Github</a>
///

#include <algorithm>

#include "algo.hpp"
#include "gtest/gtest.h"

using namespace std;
using namespace algo::image;
using namespace algo::image::feature;
using namespace algo::image::object;

namespace {

Img GetCornerImg()
{
  Data8 data{
      91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 91, 92, 91, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 92, 91, 91, 91, 92, 91, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 89, 89, 90, 89, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 89, 90, 89, 90, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 49, 21, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 31, 32, 14, 66, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 60, 94, 90, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 40, 65, 62, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 62, 26, 0, 63, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 59, 98, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 93, 36, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 56, 93, 89, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 90, 91, 89, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 57, 94, 90, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 92, 90, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 56, 93, 89, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 90, 91, 89, 35, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 58, 97, 93, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 92, 36, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 42, 0, 48, 80, 77, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 78, 76, 31, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 39, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 0, 62, 94, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 92, 39, 0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 0, 61, 94, 90, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 83, 81, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 82, 81, 86, 92, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 93, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 94, 92, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 90, 91, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91,
      91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91, 91};

  return Img{data, Size{75, 47}};
}
}// namespace

/////////////////////////////////////////////
/// Corners
/////////////////////////////////////////////

TEST(test_algo_image, test_corners)
{
  Img corner_img{GetCornerImg()};
  GaussWindowSettings g_win_set{Size{5, 5}, 1.0};
  Points pts{Corners(corner_img, 1e6, CornerDetType::kHarris, 8, 5, g_win_set)};
  EXPECT_FALSE(pts.empty());
  pts = Corners(corner_img, 1e2, CornerDetType::kShiTomasi, 8, 5, g_win_set);
  EXPECT_FALSE(pts.empty());
}