#pragma once
#include "glm/glm.hpp"
#include "ofColor.h"
#include "ofImage.h"
#include "ColorSpace.h"
#include "Conversion.h"
#include "Comparison.h"

#include "ofLog.h"

using namespace glm;

namespace ofxColorSpace {

using Rgb       = ColorSpace::Rgb;
using Xyz       = ColorSpace::Xyz;
using Yxy       = ColorSpace::Yxy;
using Cmy       = ColorSpace::Cmy;
using Cmyk      = ColorSpace::Cmyk;
using Luv       = ColorSpace::Luv;
using Lch       = ColorSpace::Lch;
using Lab       = ColorSpace::Lab;
using Hsl       = ColorSpace::Hsl;
using Hsb       = ColorSpace::Hsb;
using Hsv       = ColorSpace::Hsv;
using HunterLab = ColorSpace::HunterLab;

template <class T>
std::vector<ofFloatColor> getGradientBy(const ofFloatColor& c1, const ofFloatColor& c2, int num) {
  Rgb rgb1(c1.r, c1.g, c1.b);
  Rgb rgb2(c2.r, c2.g, c2.b);

  T t1;
  T t2;

  rgb1.To<T>(&t1);
  rgb2.To<T>(&t2);

  std::vector<ofFloatColor> gradient;

  if (num <= 1) {
    ofLogWarning() << "gradient num should be greater than 1";
    return gradient;
  }

  float step = 1. / float(num - 1);

  for (int i = 0; i < num; i++) {
    vec3 c = mix(t1.getComponent(), t2.getComponent(), step * i);
    T    t(c.x, c.y, c.z);
    Rgb  rgb;
    t.To<Rgb>(&rgb);

    gradient.push_back(ofFloatColor(rgb.r, rgb.g, rgb.b));
  }

  return gradient;
}

template <class T>
ofFloatPixels getPixelsBy(const ofFloatColor& c1, const ofFloatColor& c2, int num) {
  const auto&   cols = getGradientBy<T>(c1, c2, num);
  ofFloatPixels pix;
  pix.allocate(num, 1, OF_IMAGE_COLOR);
  for (int i = 0; i < num; i++) {
    pix[i * 3 + 0] = cols[i].r;
    pix[i * 3 + 1] = cols[i].g;
    pix[i * 3 + 2] = cols[i].b;
  }
  return pix;
}

enum COMPARISON {
  Euclidean,
  Cie1976,
  Cie94,
  Cie2000,
  Cmc
};
const vector<string> COMPARISON_LIST = {
    "Euclidean",
    "Cie1976",
    "Cie94",
    "Cie2000",
    "Cmc"};

inline double getComparison(const ofFloatColor& c1, const ofFloatColor& c2, COMPARISON method = COMPARISON::Cie2000) {
  ColorSpace::Rgb a(c1.r, c1.g, c1.b);
  ColorSpace::Rgb b(c2.r, c2.g, c2.b);
  //   ColorSpace::Cie2000Comparison::Compare(&a, &b);
  switch (method) {
    case Euclidean: return ColorSpace::EuclideanComparison::Compare(&a, &b);
    case Cie1976: return ColorSpace::Cie1976Comparison::Compare(&a, &b);
    case Cie94: return ColorSpace::Cie94Comparison::Compare(&a, &b);
    case Cie2000: return ColorSpace::Cie2000Comparison::Compare(&a, &b);
    case Cmc: return ColorSpace::CmcComparison::Compare(&a, &b);
  }
  return 1.0;
}

};  // namespace ofxColorSpace