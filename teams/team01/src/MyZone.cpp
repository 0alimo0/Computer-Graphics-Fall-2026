#include "Plot.h"

#include <cmath>
#include <optional>

namespace team01 {
namespace {

void addObservatoryColliders(citylab::Context& ctx, const PlotLayout& layout) {
    // The SDK uses boxes for collision. Narrow strips follow the circular wall.
    constexpr int strips = 11;
    constexpr float radius = PlotLayout::observatoryRadius;
    constexpr float depth = 2.0f * radius / strips;
    for (int strip = 0; strip < strips; ++strip) {
        const float back = -radius + strip * depth;
        const float middle = back + depth * 0.5f;
        const float halfWidth = std::sqrt(radius * radius - middle * middle);
        ctx.addCollider({layout.observatoryX - halfWidth, 0.0f, layout.observatoryZ + back},
                        {layout.observatoryX + halfWidth, PlotLayout::observatoryRoofY,
                         layout.observatoryZ + back + depth});
    }
}

}  // namespace

class ObservatoryZone : public citylab::Zone {
public:
    void init(citylab::Context& ctx) override {
        const auto& layout = layout_.emplace(ctx.plot());
        addSiteColliders(ctx, layout);
        addObservatoryColliders(ctx, layout);
    }

    void draw(const citylab::Context&) const override {
        const auto& layout = *layout_;
        drawSite(layout);

        glPushMatrix();
        glTranslatef(layout.observatoryX, 0.0f, layout.observatoryZ);
        drawObservatory();
        glPopMatrix();
    }

private:
    std::optional<PlotLayout> layout_;
};

}  // namespace team01

CITYLAB_REGISTER_ZONE(team01::ObservatoryZone);
