#include "ColorScheme.hpp"
#include <cmath>

using std::fabs;

void SoftColor::writeColor(color_t& data, float min, float max) {
    float x = data.dataValue;

    float t = x / (max);

    if(t < 0.34) {
        data.rgba.r = 155 + (unsigned char)(292.0 * t); //(unsigned char) (255.0*(0.61 + (0.39*t/0.34)));
        data.rgba.g = 188 + (unsigned char)(180.0 * t);// (unsigned char) (255.0*(0.74 + (0.24*t/0.34)));
        data.rgba.b = 255;
    } else if(t <= 0.8) {
        data.rgba.r = 255;
        data.rgba.g = 250 - (unsigned char)(236.0 * (t-0.34)); //(255.0 * (0.98 - (0.427*(t-0.34)/0.46)));
        data.rgba.b = 255 - (unsigned char)(554.0 * (t-0.34)); //1.0 -(t-0.34)/0.46;
    } else {
        data.rgba.r = 255;
        data.rgba.g = 141 - (unsigned char)(450.0 * (t-0.8));
        data.rgba.b = 0;
    }

    data.rgba.a = (unsigned char)(255*fabs(0.0f - x)/max);
}


void ColdColor::writeColor(color_t& data, float min, float max) {
    float point = (data.dataValue - min) / (max - min);

    if (point <= 0.2) {
        data.rgba.r = (1 - 5 * point) * 255;
        data.rgba.g = 0;
        data.rgba.b = 255;
    } else if (point > 0.2 && point <= 0.4) {
        data.rgba.r = 0;
        data.rgba.g = (5 * point - 1) * 255;
        data.rgba.b = 255;
    } else if (point > 0.4 && point <= 0.6) {
        data.rgba.r = 0;
        data.rgba.g = 255;
        data.rgba.b = (3 - 5 * point) * 255;
    } else if (point > 0.6 && point <= 0.8) {
        data.rgba.r = (5 * point - 3) * 255;
        data.rgba.g = (4 - 5 * point) * 255;
        data.rgba.b = 0;
    } else if (point > 0.8 && point <= 1) {
        data.rgba.r = 255;
        data.rgba.g = (5 * point - 4) * 255;
        data.rgba.b = 0;
    } else if (point == 0.0) {
        data.rgba.r = 100;
        data.rgba.g = 100;
        data.rgba.b = 100;
    }

    data.rgba.a = 0;
}

ColorSchemeI* ColorSchemeFactory::getScheme(const std::string& schemeName) {
    if(schemeName == softScheme) {
        return new SoftColor();
    } else if(schemeName == coldScheme) {
        return new ColdColor();
    }

    return new SoftColor();
}
