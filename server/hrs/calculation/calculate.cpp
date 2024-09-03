#include "calculate.hpp"

#include <cmath>
#include <corecrt_math_defines.h>
#include <iostream>

// 60 - 0.238379 nm/mm
// 65 - 0.201487 nm/mm
// 70 - 0.163061 nm/mm
// 75 - 0.123394 nm/mm

static double to_degrees(double rad) { return rad * 180.0 / M_PI; }
static double to_rad(double degrees) { return degrees / 180.0 * M_PI; }

#include <vector>
using namespace std;

// todo constexpr
void calculateDisp(std::vector<float> &disps) {
    std::vector<float> angs = {60, 65, 70, 75};
    for (auto &ang : angs) {
        float stepGrate;
        stepGrate = 475.22983; // nm
        float f = 500;         // mm
        float disp1 = stepGrate * cos(to_rad(ang)) / 2 / f;
        stepGrate = 478.28725; // nm
        float disp2 = stepGrate * cos(to_rad(ang)) / 2 / f;
        disps.push_back((disp1 + disp2) / 2);
    }
}

// todo constexpr
float calculateAngle(float grateAngle, float wavelength) {
    float stepGrate;
    stepGrate = 475.22983; // nm
    float mirrorAngle1 =
        grateAngle -
        to_degrees(asin(wavelength / stepGrate - sin(to_rad(grateAngle))));
    stepGrate = 478.28725; // nm
    float mirrorAngle2 =
        grateAngle -
        to_degrees(asin(wavelength / stepGrate - sin(to_rad(grateAngle))));

    if (mirrorAngle1 > 87 || mirrorAngle1 < 30) {
        return 60;
    }

    float a;
    float b;
    float delta;
    a = 0.13738;  // 3517.0 * 3 / 51200; // 0.13738
    b = -0.12078; //-3092.0 * 3 / 51200; // -0.12078
    delta = a + b * cos(to_rad(grateAngle));

    return (mirrorAngle1 + mirrorAngle2) / 2 - delta;
}
