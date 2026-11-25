#pragma once

#include <citylab/CityLab.h>

namespace team01 {

struct PlotLayout {
    explicit PlotLayout(const citylab::PlotInfo& plot)
        : halfWidth(plot.width * 0.5f),
          halfDepth(plot.depth * 0.5f),
          gateWidth(plot.gateWidth),
          observatoryX(-0.50f * halfWidth),
          observatoryZ(-0.36f * halfDepth),
          planetariumX(0.50f * halfWidth),
          planetariumZ(-0.22f * halfDepth) {}

    static constexpr float observatoryHalfWidth = 11.0f;
    static constexpr float observatoryHalfDepth = 8.5f;
    static constexpr float planetariumHalfWidth = 14.0f;
    static constexpr float planetariumHalfDepth = 16.0f;
    static constexpr float plazaZ = 19.0f;

    float halfWidth;
    float halfDepth;
    float gateWidth;
    float observatoryX;
    float observatoryZ;
    float planetariumX;
    float planetariumZ;
};

void drawSite(const PlotLayout& layout);
void drawObservatory();

}  // namespace team01
