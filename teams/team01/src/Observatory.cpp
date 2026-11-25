#include "Plot.h"

#include <cmath>

namespace team01 {
namespace {

using namespace citylab;

constexpr float buildingRadius = PlotLayout::observatoryRadius;
constexpr float roofY = PlotLayout::observatoryRoofY;
constexpr float domeBase = roofY + 0.85f;
constexpr float telescopePivotY = domeBase + 6.35f;
constexpr float pi = 3.14159265358979323846f;

void boxAt(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);
    shapes::box(width, height, depth);
    glPopMatrix();
}

void cylinderAt(float y, float radius, float height) {
    glPushMatrix();
    glTranslatef(0.0f, y, 0.0f);
    shapes::cylinder(radius, height, 64);
    glPopMatrix();
}

void drawWindow() {
    glColor3f(0.14f, 0.21f, 0.29f);
    shapes::box(2.6f, 2.05f, 0.42f);
    glColor3f(0.25f, 0.54f, 0.67f);
    boxAt(0.0f, 0.0f, 0.23f, 2.32f, 1.78f, 0.08f);
    glColor3f(0.72f, 0.80f, 0.84f);
    boxAt(0.0f, 0.0f, 0.28f, 0.09f, 1.78f, 0.06f);
}

void drawWindowRing(float y, float firstAngle, int count) {
    glPushMatrix();
    glTranslatef(0.0f, y, 0.0f);
    for (int window = 0; window < count; ++window) {
        glPushMatrix();
        glRotatef(firstAngle + window * (360.0f / count), 0.0f, 1.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, buildingRadius - 0.37f);
        drawWindow();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawEntrance() {
    constexpr float porch = PlotLayout::entranceDepth;
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, buildingRadius);
    glColor3f(0.55f, 0.60f, 0.64f);
    boxAt(0.0f, 0.12f, (porch - 0.65f) * 0.5f, 5.8f, 0.24f, porch + 0.65f);
    glColor3f(0.16f, 0.24f, 0.32f);
    boxAt(0.0f, 1.85f, -0.35f, 3.9f, 3.22f, 0.98f);
    boxAt(0.0f, 3.62f, 0.45f, 5.8f, 0.44f, 2.6f);

    glColor3f(0.26f, 0.52f, 0.64f);
    for (const float side : {-1.0f, 1.0f}) {
        boxAt(side * 0.83f, 1.76f, 0.17f, 1.50f, 2.86f, 0.10f);
    }
    glPopMatrix();
}

void drawBuilding() {
    glColor3f(0.36f, 0.42f, 0.48f);
    cylinderAt(0.0f, buildingRadius, 0.54f);
    glColor3f(0.83f, 0.88f, 0.91f);
    cylinderAt(0.54f, buildingRadius - 0.35f, roofY - 1.24f);
    glColor3f(0.23f, 0.32f, 0.40f);
    cylinderAt(7.25f, buildingRadius - 0.11f, 0.37f);
    cylinderAt(roofY - 0.70f, buildingRadius, 0.70f);
    glColor3f(0.34f, 0.67f, 0.73f);
    cylinderAt(roofY - 0.32f, buildingRadius + 0.03f, 0.09f);

    drawWindowRing(4.45f, 15.0f, 12);
    drawWindowRing(10.90f, 0.0f, 12);
    drawEntrance();
}

constexpr float domeRadius = 13.55f;
constexpr float domeHeight = 11.55f;
constexpr float shellInner = 0.965f;

void domeVertex(float across, float along, float shell, float normalSign) {
    const float x = std::sin(across);
    const float y = std::cos(across) * std::sin(along);
    const float z = std::cos(across) * std::cos(along);
    glNormal3f(normalSign * x / domeRadius, normalSign * y / domeHeight,
               normalSign * z / domeRadius);
    glVertex3f(shell * domeRadius * x, shell * domeHeight * y, shell * domeRadius * z);
}

void drawDomePanel(float left, float right, int firstRow, int columns, bool shutter) {
    constexpr int rows = 36;
    constexpr float rowStep = pi / rows;
    glBegin(GL_QUADS);
    for (int column = 0; column < columns; ++column) {
        const float a = left + (right - left) * column / columns;
        const float b = left + (right - left) * (column + 1) / columns;
        const float shade = column % 4 == 0 ? 0.09f : 0.0f;
        for (int row = firstRow; row < rows; ++row) {
            const float near = row * rowStep;
            const float far = (row + 1) * rowStep;

            glColor3f(0.87f - shade, 0.92f - shade, 0.96f - shade);
            domeVertex(a, near, 1.0f, 1.0f);
            domeVertex(b, near, 1.0f, 1.0f);
            domeVertex(b, far, 1.0f, 1.0f);
            domeVertex(a, far, 1.0f, 1.0f);

            glColor3f(0.21f, 0.27f, 0.34f);
            domeVertex(a, far, shellInner, -1.0f);
            domeVertex(b, far, shellInner, -1.0f);
            domeVertex(b, near, shellInner, -1.0f);
            domeVertex(a, near, shellInner, -1.0f);
        }

        for (const float edge : {firstRow * rowStep, pi}) {
            // The rear shutter edge is shared with the adjacent panels.
            glColor3f(shutter && edge < pi ? 0.35f : 0.49f, 0.59f, 0.67f);
            domeVertex(a, edge, 1.0f, 1.0f);
            domeVertex(b, edge, 1.0f, 1.0f);
            domeVertex(b, edge, shellInner, 1.0f);
            domeVertex(a, edge, shellInner, 1.0f);
        }
    }
    glEnd();
}

void drawSlitRail(float across, int slitRows) {
    constexpr float rowStep = pi / 36.0f;
    const float outside = across + std::copysign(0.019f, across);
    glBegin(GL_QUADS);
    for (int row = 0; row < slitRows; ++row) {
        const float near = row * rowStep;
        const float far = (row + 1) * rowStep;
        glColor3f(0.23f, 0.36f, 0.46f);
        domeVertex(across, near, shellInner, 1.0f);
        domeVertex(across, far, shellInner, 1.0f);
        domeVertex(across, far, 1.01f, 1.0f);
        domeVertex(across, near, 1.01f, 1.0f);
        glColor3f(0.43f, 0.72f, 0.77f);
        domeVertex(across, near, 1.01f, 1.0f);
        domeVertex(across, far, 1.01f, 1.0f);
        domeVertex(outside, far, 1.01f, 1.0f);
        domeVertex(outside, near, 1.01f, 1.0f);
    }
    glEnd();
}

void drawDome() {
    constexpr float slitEdge = 0.32f;
    constexpr int slitRows = 25;

    glPushMatrix();
    glTranslatef(0.0f, roofY - 0.02f, 0.0f);
    glColor3f(0.25f, 0.33f, 0.43f);
    shapes::cylinder(domeRadius + 0.20f, 0.54f, 64);
    glTranslatef(0.0f, 0.54f, 0.0f);
    glColor3f(0.67f, 0.78f, 0.82f);
    shapes::cylinder(domeRadius + 0.10f, domeBase - roofY - 0.52f, 64);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, domeBase, 0.0f);
    drawDomePanel(-0.5f * pi, -slitEdge, 0, 14, false);
    drawDomePanel(slitEdge, 0.5f * pi, 0, 14, false);
    drawDomePanel(-slitEdge, slitEdge, slitRows, 8, true);
    drawSlitRail(-slitEdge, slitRows);
    drawSlitRail(slitEdge, slitRows);
    glPopMatrix();
}

void drawTelescopeMount() {
    glPushMatrix();
    glTranslatef(0.0f, telescopePivotY - 4.2f, 0.0f);
    glColor3f(0.18f, 0.24f, 0.33f);
    shapes::cylinder(2.95f, 2.1f, 32);
    glColor3f(0.77f, 0.64f, 0.44f);
    glTranslatef(0.0f, 2.1f, 0.0f);
    shapes::cylinder(2.45f, 0.34f, 32);
    glPopMatrix();

    glColor3f(0.22f, 0.29f, 0.39f);
    for (const float side : {-1.0f, 1.0f}) {
        boxAt(side * 2.55f, telescopePivotY - 0.95f, 0.0f, 0.72f, 2.15f, 1.35f);
        glPushMatrix();
        glTranslatef(side * 2.89f, telescopePivotY, 0.0f);
        glRotatef(-side * 90.0f, 0.0f, 0.0f, 1.0f);
        glColor3f(0.86f, 0.68f, 0.39f);
        shapes::cylinder(0.58f, 0.32f, 20);
        glColor3f(0.22f, 0.29f, 0.39f);
        glPopMatrix();
    }
}

void drawOpticalTube() {
    // The tube's local +y axis points toward the objective lens.
    glColor3f(0.87f, 0.86f, 0.79f);
    glPushMatrix();
    glTranslatef(0.0f, -5.6f, 0.0f);
    shapes::cylinder(1.84f, 17.5f, 40);
    glPopMatrix();

    for (const float position : {-5.9f, -2.4f, 1.7f, 8.5f, 11.3f}) {
        glPushMatrix();
        glTranslatef(0.0f, position, 0.0f);
        glColor3f(0.22f, 0.30f, 0.41f);
        shapes::cylinder(1.96f, 0.58f, 40);
        glColor3f(0.82f, 0.66f, 0.41f);
        glTranslatef(0.0f, 0.39f, 0.0f);
        shapes::cylinder(1.99f, 0.09f, 40);
        glPopMatrix();
    }

    glPushMatrix();
    glTranslatef(0.0f, -7.4f, 0.0f);
    glColor3f(0.28f, 0.34f, 0.43f);
    shapes::cylinder(0.79f, 1.6f, 28);
    glColor3f(0.75f, 0.62f, 0.41f);
    shapes::cylinder(1.06f, 0.32f, 28);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 11.82f, 0.0f);
    glColor3f(0.19f, 0.26f, 0.36f);
    shapes::cylinder(2.22f, 1.58f, 48, false);
    glColor3f(0.07f, 0.16f, 0.25f);
    shapes::cylinder(2.08f, 1.58f, 48, false);

    glTranslatef(0.0f, 0.20f, 0.0f);
    glColor3f(0.10f, 0.38f, 0.60f);
    shapes::disk(2.09f, 48);
    glTranslatef(0.0f, 0.012f, 0.0f);
    glColor3f(0.18f, 0.54f, 0.69f);
    shapes::disk(1.59f, 48);
    glTranslatef(0.0f, 1.368f, 0.0f);
    glColor3f(0.82f, 0.67f, 0.42f);
    shapes::torus(2.15f, 0.13f, 48, 10);
    glPopMatrix();
}

void drawTelescope() {
    glPushMatrix();
    drawTelescopeMount();
    glTranslatef(0.0f, telescopePivotY, 0.0f);
    glRotatef(53.0f, 1.0f, 0.0f, 0.0f);
    glScalef(1.28f, 1.32f, 1.28f);
    drawOpticalTube();
    glPopMatrix();
}

}  // namespace

void drawObservatory() {
    drawBuilding();
    // The dome slit and the telescope use the same heading.
    glPushMatrix();
    glRotatef(-35.0f, 0.0f, 1.0f, 0.0f);
    drawDome();
    drawTelescope();
    glPopMatrix();
}

}  // namespace team01
