#include "Plot.h"

#include <array>
#include <cmath>

namespace team01 {
namespace {

using namespace citylab;

constexpr float grassY = 0.0f;
constexpr float curbY = 0.025f;
constexpr float pavingY = 0.04f;
constexpr float junctionHalfDepth = 2.1f;
constexpr float wallHeight = 0.95f;
constexpr float wallThickness = 0.50f;

struct WallSection {
    float left;
    float right;
    float back;
    float front;
};

std::array<WallSection, 5> boundarySections(const PlotLayout& layout) {
    constexpr float setback = 0.30f;
    const float x = layout.halfWidth - setback;
    const float z = layout.halfDepth - setback;
    const float gate = layout.gateWidth * 0.5f + setback;
    return {{{-x, x, -z, -z + wallThickness},
             {-x, -x + wallThickness, -z + wallThickness, z - wallThickness},
             {x - wallThickness, x, -z + wallThickness, z - wallThickness},
             {-x, -gate, z - wallThickness, z},
             {gate, x, z - wallThickness, z}}};
}

void drawBoundary(const PlotLayout& layout) {
    for (const auto& section : boundarySections(layout)) {
        const float width = section.right - section.left;
        const float depth = section.front - section.back;
        glPushMatrix();
        glTranslatef((section.left + section.right) * 0.5f, 0.0f,
                     (section.back + section.front) * 0.5f);
        glTranslatef(0.0f, 0.40f, 0.0f);
        glColor3f(0.62f, 0.66f, 0.69f);
        shapes::box(width, 0.80f, depth);
        glTranslatef(0.0f, 0.475f, 0.0f);
        glColor3f(0.23f, 0.29f, 0.36f);
        shapes::box(width, 0.15f, depth);
        glPopMatrix();
    }
}

void surface(float x, float y, float z, float width, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);
    shapes::plane(width, depth);
    glPopMatrix();
}

void paving(float left, float right, float back, float front) {
    const float centerX = (left + right) * 0.5f;
    const float width = right - left;
    constexpr float edge = 0.22f;
    constexpr float joint = 0.035f;
    constexpr float slabDepth = 2.0f;

    glColor3f(0.47f, 0.51f, 0.56f);
    surface(centerX, curbY, (back + front) * 0.5f, width, front - back);

    const int rows = static_cast<int>(std::ceil((front - back) / slabDepth));
    const float rowDepth = (front - back) / static_cast<float>(rows);
    for (int row = 0; row < rows; ++row) {
        const float shade = row % 2 == 0 ? 0.74f : 0.71f;
        glColor3f(shade, shade + 0.025f, shade + 0.04f);
        surface(centerX, pavingY, back + (static_cast<float>(row) + 0.5f) * rowDepth,
                width - 2.0f * edge, rowDepth - joint);
    }
}

void drawPathLight(float x, float z, float yaw) {
    glPushMatrix();
    glTranslatef(x, grassY, z);
    glRotatef(yaw, 0.0f, 1.0f, 0.0f);
    glColor3f(0.18f, 0.22f, 0.29f);
    shapes::cylinder(0.27f, 0.14f, 10);
    shapes::cylinder(0.09f, 1.15f, 10);
    glTranslatef(0.0f, 1.15f, 0.0f);
    glColor3f(0.93f, 0.83f, 0.56f);
    shapes::box(0.30f, 0.18f, 0.24f);
    glTranslatef(0.0f, 0.11f, 0.035f);
    glColor3f(0.18f, 0.22f, 0.29f);
    shapes::box(0.39f, 0.10f, 0.37f);
    glPopMatrix();
}

void markPlanetariumCorners(const PlotLayout& layout) {
    constexpr float lineWidth = 0.28f;
    constexpr float armLength = 2.5f;
    glColor3f(0.78f, 0.74f, 0.52f);
    for (const float xSide : {-1.0f, 1.0f}) {
        for (const float zSide : {-1.0f, 1.0f}) {
            const float x = layout.planetariumX + xSide * PlotLayout::planetariumHalfWidth;
            const float z = layout.planetariumZ + zSide * PlotLayout::planetariumHalfDepth;
            surface(x - xSide * armLength * 0.5f, curbY, z, armLength, lineWidth);
            surface(x, pavingY, z - zSide * armLength * 0.5f, lineWidth, armLength);
        }
    }
}

}  // namespace

void addSiteColliders(citylab::Context& ctx, const PlotLayout& layout) {
    for (const auto& section : boundarySections(layout)) {
        ctx.addCollider({section.left, 0.0f, section.back},
                        {section.right, wallHeight, section.front});
    }
}

void drawSite(const PlotLayout& layout) {
    constexpr int grassStripes = 16;
    const float stripeWidth = 2.0f * layout.halfWidth / grassStripes;
    for (int i = 0; i < grassStripes; ++i) {
        const float shade = i % 2 == 0 ? 0.0f : 0.025f;
        glColor3f(0.26f + shade, 0.43f + shade, 0.24f + shade);
        surface(-layout.halfWidth + (static_cast<float>(i) + 0.5f) * stripeWidth,
                grassY, 0.0f, stripeWidth, 2.0f * layout.halfDepth);
    }

    constexpr float junctionFront = PlotLayout::junctionZ + junctionHalfDepth;
    constexpr float junctionBack = PlotLayout::junctionZ - junctionHalfDepth;
    const float entryHalfWidth = layout.gateWidth * 0.24f;
    paving(-entryHalfWidth, entryHalfWidth, junctionFront, layout.halfDepth);
    constexpr float branchHalfWidth = 1.8f;
    paving(layout.observatoryX - branchHalfWidth, layout.planetariumX + branchHalfWidth,
           junctionBack, junctionFront);

    paving(layout.observatoryX - branchHalfWidth, layout.observatoryX + branchHalfWidth,
           layout.observatoryZ + PlotLayout::observatoryRadius + PlotLayout::entranceDepth, junctionBack);
    paving(layout.planetariumX - branchHalfWidth, layout.planetariumX + branchHalfWidth,
           layout.planetariumZ + PlotLayout::planetariumHalfDepth, junctionBack);

    markPlanetariumCorners(layout);
    drawBoundary(layout);

    for (const float side : {-1.0f, 1.0f}) {
        for (int i = 1; i <= 3; ++i) {
            const float z = junctionFront + (layout.halfDepth - junctionFront) * (0.25f * i);
            drawPathLight(side * (entryHalfWidth + 0.7f), z, side * 90.0f);
        }
    }
}

}  // namespace team01
