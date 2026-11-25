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
          planetariumZ(observatoryZ) {}

    static constexpr float observatoryRadius = 16.0f;
    static constexpr float observatoryRoofY = 17.2f;
    static constexpr float entranceDepth = 1.9f;
    static constexpr float planetariumHalfWidth = 14.0f;
    static constexpr float planetariumHalfDepth = 16.0f;
    static constexpr float junctionZ = 19.0f;

    float halfWidth;
    float halfDepth;
    float gateWidth;
    float observatoryX;
    float observatoryZ;
    float planetariumX;
    float planetariumZ;
};

void drawSite(const PlotLayout& layout);
void addSiteColliders(citylab::Context& ctx, const PlotLayout& layout);
void drawObservatory();

}  // namespace team01
