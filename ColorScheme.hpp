#ifndef COLORSCHEME_HPP
#define COLORSCHEME_HPP

#include <string>

const std::string softScheme = "soft";
const std::string coldScheme = "cold";

union color_t {
    color_t(float value) : dataValue(value) { }

    struct {unsigned char r, g, b, a;} rgba;
    float dataValue;
};

class ColorSchemeI {
    public : virtual void writeColor(color_t& data, float min, float max) = 0;
};

class SoftColor : public ColorSchemeI {
    public : void writeColor(color_t& data, float min, float max);
};

class ColdColor : public ColorSchemeI {
    public : void writeColor(color_t& data, float min, float max);
};


class ColorSchemeFactory {
    public : static ColorSchemeI* getScheme(const std::string& schemeName);
};

#endif // COLORSCHEME_HPP
