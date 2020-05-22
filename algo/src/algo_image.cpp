///
/// \brief Source file for image algorithms.
/// \author alex011235
/// \date 2020-05-18
/// \link <a href=https://github.com/alex011235/algorithm>Algorithm, Github</a>
///

#include "algo_image.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

namespace algo::image {

/////////////////////////////////////////////
/// Image functions
/////////////////////////////////////////////

uint8_t Img::At(const int& x, const int& y) const
{
  return data[y * size.cols + x];
}

void Img::Set(const int& x, const int& y, const uint8_t& value)
{
  data[y * size.cols + x] = value;
}

uint32_t IntegralImage::At(const int& x, const int& y) const
{
  return data[y * size.cols + x];
}

void IntegralImage::Set(const int& x, const int& y, const uint32_t& value)
{
  data[y * size.cols + x] = value;
}

uint32_t ImgF::At(const int& x, const int& y) const
{
  return data[y * size.cols + x];
}

void ImgF::Set(const int& x, const int& y, const float& value)
{
  data[y * size.cols + x] = value;
}
/////////////////////////////////////////////
/// Fundamental functions
/////////////////////////////////////////////

Img NewImgGray(const int& rows, const int& cols)
{
  Img im;
  im.size.rows = rows;
  im.size.cols = cols;
  im.data = Data8(rows * cols, 0);
  return im;
}

Img ToGray(const Img3& img3)
{
  int size{img3.size.rows * img3.size.cols};
  Data8 data(size, 0);

  for (size_t i = 0; i < size; i++) {
    data[i] = 0.3 * img3.data[Red][i] + 0.59 * img3.data[Green][i] + 0.11 * img3.data[Blue][i];
  }

  Img img{data, img3.size};
  return img;
}

Img InvertPixels(const Img& im)
{
  Img img{im};
  std::transform(img.data.begin(), img.data.end(), img.data.begin(), [](uint8_t x) {
    return 255 - x;
  });
  return img;
}

/////////////////////////////////////////////
/// Integral images
/////////////////////////////////////////////

IntegralImage ImgToIntegralImage(const Img& im)
{
  IntegralImage img{Data32(im.size.rows * im.size.cols, 0), im.size};

  for (int x = 0; x < im.size.cols; x++) {
    for (int y = 0; y < im.size.rows; y++) {

      if (x == 0 & y == 0) {
        img.Set(x, y, im.At(0, 0));

      } else if (x == 0) {
        img.Set(x, y, img.At(0, y - 1) + im.At(x, y));

      } else if (y == 0) {
        img.Set(x, y, img.At(x - 1, y) + im.At(x, y));

      } else {
        uint32_t yy{img.At(x, y - 1)};
        uint32_t xx{img.At(x - 1, y)};
        uint32_t xy{img.At(x - 1, y - 1)};
        img.Set(x, y, -xy + yy + xx + im.At(x, y));
      }
    }
  }
  return img;
}

uint32_t IntegralBoxSum(const IntegralImage& img, const Box& box)
{
  return img.At(box.x, box.y) + img.At(box.x + box.width - 1, box.y + box.height - 1) - img.At(box.x + box.width - 1, box.y) - img.At(box.x, box.y + box.height - 1);
}

/////////////////////////////////////////////
/// Filters
/////////////////////////////////////////////

namespace {

using Filter = std::array<float, 9>;

constexpr Filter filter_sobel_x{-1.0 / 2.0, 0, 1.0 / 2.0, -2.0 / 2.0, 0, 2.0 / 2.0, -1.0 / 2.0, 0, 1.0 / 2.0};
constexpr Filter filter_sobel_y{-1.0 / 2.0, -2.0 / 2.0, -1.0 / 2.0, 0, 0, 0, 1.0 / 2.0, 2.0 / 2.0, 1.0 / 2.0};
constexpr Filter filter_edge{1.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0};
//constexpr Filter filter_edge{-1.0, -1.0, -1.0, -1.0, 8.0, -1.0, -1.0, -1.0, -1.0};
constexpr Filter filter_smooth{1.0 / 13.0, 2.0 / 13.0, 1.0 / 13.0, 2.0 / 13.0, 4.0 / 13.0, 2.0 / 13.0, 1.0 / 13.0, 2.0 / 13.0, 1.0 / 13.0};
constexpr Filter filter_sharp_agg{0, -1.0, 0, -1.0, 5.0, -1.0, 0, -1.0, 0};
constexpr Filter filter_sharp_mod{-1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0, 17.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0, -1.0 / 9.0};
constexpr Filter filter_gauss_blur{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 5.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0};
constexpr Filter filter_blur_hard{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0};
constexpr Filter filter_blur_soft{0.0, 1.0 / 8.0, 0.0, 1.0 / 8.0, 1.0 / 2.0, 1.0 / 8.0, 0.0, 1.0 / 8.0, 0.0};
constexpr Filter filter_emboss{-2.0, -1.0, 0.0, -1.0, 1.0, 1.0, 0.0, 1.0, 2.0};
constexpr Filter filter_weighted{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0};
constexpr Filter filter_dilation_v{0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0};
constexpr Filter filter_dilation_h{0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0};
constexpr Filter filter_dilation{0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0};
constexpr Filter filter_high_pass{-1.0, -1.0, -1.0, -1.0, 8.0, -1.0, -1.0, -1.0, -1.0};
constexpr Filter filter_nothing{0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0};

Filter GetFilter(const FilterType& filter_type)
{
  switch (filter_type) {
    case FilterType::SOBEL_X:
      return filter_sobel_x;
    case FilterType::SOBEL_Y:
      return filter_sobel_y;
    case FilterType::EDGE_DETECT:
      return filter_edge;
    case FilterType::SMOOTHING:
      return filter_smooth;
    case FilterType::SHARPEN_MODEST:
      return filter_sharp_mod;
    case FilterType::SHARPEN_AGGRESSIVE:
      return filter_sharp_agg;
    case FilterType::GAUSSIAN_BLUR:
      return filter_gauss_blur;
    case FilterType::BLUR_HARD:
      return filter_blur_hard;
    case FilterType::BLUR_SOFT:
      return filter_blur_soft;
    case FilterType::EMBOSS:
      return filter_emboss;
    case FilterType::WEIGHTED_AVERAGE:
      return filter_weighted;
    case FilterType::DILATION_VERTICAL:
      return filter_dilation_v;
    case FilterType::DILATION_HORIZONTAL:
      return filter_dilation_h;
    case FilterType::DILATION:
      return filter_dilation;
    case FilterType::HIGH_PASS:
      return filter_high_pass;
  }
  return filter_nothing;
}

}// namespace

/////////////////////////////////////////////
/// Convolutions
/////////////////////////////////////////////

Data8 ConvolvePriv(const Data8& im, size_t rows, size_t cols, FilterType filter_type)
{
  Data8 res(im.size(), 0);
  const Filter filter{GetFilter(filter_type)};
  uint8_t dim{3U};
  uint8_t sz{static_cast<uint8_t>(dim >> 1U)};

  // Filtering window
  for (size_t i = sz; i < rows - sz; i++) {
    for (size_t j = sz; j < cols - sz; j++) {
      float sum = 0;
      for (uint8_t k = 0; k < dim; k++) {
        for (uint8_t m = 0; m < dim; m++) {
          // Multiply components
          auto im_num = static_cast<float>((im[(i + k - sz) * cols + (j + m - sz)]));
          float filt_num = filter[k * dim + m];
          sum += im_num * filt_num;
        }
      }
      // Check overflow
      if (sum < 0) sum = 0;
      if (sum > 255) sum = 255;

      res[i * cols + j] = sum;
    }
  }
  return res;
}

Img Convolve(const Img& im, FilterType filter_type)
{
  Img res{im};
  res.data = ConvolvePriv(res.data, im.size.rows, im.size.cols, filter_type);
  return res;
}

Img3 Convolve3(const Img3& im, FilterType filter_type)
{
  Img3 res{im};
  res.data[Red] = ConvolvePriv(res.data[Red], im.size.rows, im.size.cols, filter_type);
  res.data[Green] = ConvolvePriv(res.data[Green], im.size.rows, im.size.cols, filter_type);
  res.data[Blue] = ConvolvePriv(res.data[Blue], im.size.rows, im.size.cols, filter_type);
  return res;
}

/////////////////////////////////////////////
/// Median filters
/////////////////////////////////////////////

Data8 MedianFilterPriv(const Data8& im, const int& rows, const int& cols, const int& w_width, const int& w_height)
{
  Data8 res(rows * cols, 0);
  int edge_x{w_width / 2};
  int edge_y{w_height / 2};

  for (size_t x = 0; x < cols - edge_x; x++) {
    for (size_t y = 0; y < rows - edge_y; y++) {

      std::vector<int> window(w_height * w_height, 0);
      int i{0};

      for (size_t wx = 0; wx < w_width; wx++) {
        for (size_t wy = 0; wy < w_height; wy++) {
          window[i] = im[(y + wy - edge_y) * cols + (x + wx - edge_x)];
          i++;
        }
      }
      std::sort(window.begin(), window.end());
      res[y * cols + x] = window[w_width * w_height / 2];
    }
  }
  return res;
}

Img MedianFilter(const Img& im, const int& w_width, const int& w_height)
{
  Img res;
  res.data = MedianFilterPriv(im.data, im.size.rows, im.size.cols, w_width, w_height);
  res.size = im.size;
  return res;
}

Img3 MedianFilter3(const Img3& im, const int& w_width, const int& w_height)
{
  Img3 res;
  res.data[Red] = MedianFilterPriv(im.data[Red], im.size.rows, im.size.cols, w_width, w_height);
  res.data[Green] = MedianFilterPriv(im.data[Green], im.size.rows, im.size.cols, w_width, w_height);
  res.data[Blue] = MedianFilterPriv(im.data[Blue], im.size.rows, im.size.cols, w_width, w_height);
  res.size = im.size;
  return res;
}

/////////////////////////////////////////////
/// Thresholding
/////////////////////////////////////////////

namespace threshold {

Img Fixed(const Img& im, const uint8_t& threshold, const bool& cut_white)
{
  Img img{im};

  std::for_each(img.data.begin(), img.data.end(), [threshold, cut_white](uint8_t& x) {
    if (x > threshold) {
      x = cut_white ? 255 : 0;
    } else {
      x = cut_white ? 0 : 255;
    }
  });
  return img;
}

Img Adaptive(const Img& im, const int& region_size, const bool& cut_white)
{
  IntegralImage img{ImgToIntegralImage(im)};
  Img res{Data8(im.size.rows * im.size.cols, 0), im.size};

  const int kMargin{region_size / 2};

  for (int x = kMargin; x < im.size.cols - kMargin; x++) {
    for (int y = kMargin; y < img.size.rows - kMargin; y++) {

      Box box{x - kMargin, y - kMargin, region_size, region_size};
      uint32_t box_sum{IntegralBoxSum(img, box)};
      uint32_t avg = box_sum / (region_size * region_size);

      if (im.At(x, y) > avg) {
        res.Set(x, y, 255);
      }
    }
  }

  if (!cut_white) {
    std::for_each(res.data.begin(), res.data.end(), [](uint8_t& x) { x = x == 0 ? 255 : 0; });
  }

  return res;
}

}// namespace threshold

namespace transform {

Img HoughLines(const Img& im)
{
  int n_rows{im.size.rows};
  int n_cols{im.size.cols};

  const int kDMax = std::sqrt(n_cols * n_cols + n_rows * n_rows);
  const int kAlphaMax{360};

  // Smooth image and run through edge detector
  Img img{Convolve(im, FilterType::EDGE_DETECT)};
  //img = Convolve(img, FilterType::EDGE_DETECT);
  img = InvertPixels(img);
  ImgF himgf{Dataf(kDMax * kAlphaMax, 0), Size{kDMax, kAlphaMax}};

  // Lambda for computing d, Klette p.125
  auto d_comp = [n_cols, n_rows](auto x, auto y, auto alpha) {
    return (x - n_cols / 2.0) * std::cos(alpha * M_PI / 180.0) + (y - n_rows / 2.0) * std::sin(alpha * M_PI / 180.0);
  };

  auto d_comp2 = [n_cols, n_rows](auto x, auto y, auto alpha) {
    return x * std::cos(alpha * M_PI / 180.0) + y * std::sin(alpha * M_PI / 180.0);
  };

  for (int x = 0; x < im.size.cols; x++) {
    for (int y = 0; y < im.size.rows; y++) {

      if (img.At(x, y) == 255) continue;

      for (int alpha = 0; alpha < 360; alpha++) {
        auto d = d_comp2(x, y, alpha);

        himgf.Set(alpha, d, himgf.At(alpha, d) + 1.0);// Vote
      }
    }
  }

  // Find maximum and then convert to uint8_t image.
  float max_elem{*std::max_element(himgf.data.begin(), himgf.data.end())};
  Img himg{Data8(kDMax * kAlphaMax, 0), Size{kDMax, kAlphaMax}};

  std::transform(himgf.data.begin(), himgf.data.end(), himg.data.begin(), [max_elem](float x) {
    return (x / max_elem) * 255.0;
  });

  return himg;
}

}// namespace transform

namespace detection {

struct h_comp {
  bool operator()(const HLine l1, const HLine l2) const
  {
    return l1.count > l2.count;
  }
} h_comp;

Hlines DetectHoughLines(const Img& im, const int& n)
{
  Hlines all_lines;

  for (int x = 0; x < im.size.cols; x++) {
    for (int y = 0; y < im.size.rows; y++) {

      if (im.At(x, y) > 200) {
        HLine line{x, y, im.At(x, y)};
        all_lines.emplace_back(line);
      }
    }
  }

  std::sort(all_lines.begin(), all_lines.end(), h_comp);

  Hlines lines(n);
  std::copy_n(all_lines.begin(), n, lines.begin());

  return lines;
}

};// namespace detection

}//namespace algo::image