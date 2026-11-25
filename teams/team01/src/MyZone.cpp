#include "Plot.h"

#include <optional>

namespace team01 {

class ObservatoryZone : public citylab::Zone {
public:
    void init(citylab::Context& ctx) override {
        const auto& layout = layout_.emplace(ctx.plot());
        ctx.addCollider({layout.observatoryX - PlotLayout::observatoryHalfWidth, 0.0f,
                         layout.observatoryZ - PlotLayout::observatoryHalfDepth},
                        {layout.observatoryX + PlotLayout::observatoryHalfWidth, 8.5f,
                         layout.observatoryZ + PlotLayout::observatoryHalfDepth});
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
