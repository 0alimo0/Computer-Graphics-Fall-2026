#include "Plot.h"

#include <cmath>

namespace team01 {
namespace {

using namespace citylab;

constexpr float grassY = 0.0f;
constexpr float curbY = 0.025f;
constexpr float pavingY = 0.04f;
constexpr float plazaHalfDepth = 5.0f;

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

void drawCompassInlay() {
    glPushMatrix();
    glTranslatef(0.0f, pavingY + 0.01f, PlotLayout::plazaZ);
    glColor3f(0.18f, 0.26f, 0.37f);
    shapes::disk(2.65f);
    glTranslatef(0.0f, 0.005f, 0.0f);
    glColor3f(0.84f, 0.69f, 0.39f);
    shapes::disk(2.48f);
    glTranslatef(0.0f, 0.005f, 0.0f);
    glColor3f(0.18f, 0.26f, 0.37f);
    shapes::disk(2.37f);

    glTranslatef(0.0f, 0.005f, 0.0f);
    glColor3f(0.91f, 0.82f, 0.60f);
    glNormal3f(0.0f, 1.0f, 0.0f);
    for (int point = 0; point < 4; ++point) {
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 0.0f, 2.08f);
        glVertex3f(0.35f, 0.0f, 0.0f);
        glVertex3f(-0.35f, 0.0f, 0.0f);
        glEnd();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    }
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

void drawSite(const PlotLayout& layout) {
    constexpr int grassStripes = 16;
    const float stripeWidth = 2.0f * layout.halfWidth / grassStripes;
    for (int i = 0; i < grassStripes; ++i) {
        const float shade = i % 2 == 0 ? 0.0f : 0.025f;
        glColor3f(0.26f + shade, 0.43f + shade, 0.24f + shade);
        surface(-layout.halfWidth + (static_cast<float>(i) + 0.5f) * stripeWidth,
                grassY, 0.0f, stripeWidth, 2.0f * layout.halfDepth);
    }

    constexpr float plazaFront = PlotLayout::plazaZ + plazaHalfDepth;
    constexpr float plazaBack = PlotLayout::plazaZ - plazaHalfDepth;
    const float entryHalfWidth = layout.gateWidth * 0.5f;
    paving(-entryHalfWidth, entryHalfWidth, plazaFront, layout.halfDepth);
    paving(layout.observatoryX - 4.5f, layout.planetariumX + 4.5f, plazaBack, plazaFront);

    constexpr float branchHalfWidth = 3.0f;
    paving(layout.observatoryX - branchHalfWidth, layout.observatoryX + branchHalfWidth,
           layout.observatoryZ + PlotLayout::observatoryHalfDepth + 1.8f, plazaBack);
    paving(layout.planetariumX - branchHalfWidth, layout.planetariumX + branchHalfWidth,
           layout.planetariumZ + PlotLayout::planetariumHalfDepth, plazaBack);

    drawCompassInlay();
    markPlanetariumCorners(layout);

    for (const float side : {-1.0f, 1.0f}) {
        for (int i = 1; i <= 3; ++i) {
            const float z = plazaFront + (layout.halfDepth - plazaFront) * (0.25f * i);
            drawPathLight(side * (entryHalfWidth + 0.7f), z, side * 90.0f);
        }
    }
}

}  // namespace team01
