#include "Plot.h"

#include <cmath>

namespace team01 {
namespace {

using namespace citylab;

constexpr float facadeZ = PlotLayout::observatoryHalfDepth;
constexpr float buildingWidth = 2.0f * PlotLayout::observatoryHalfWidth;
constexpr float buildingDepth = 2.0f * PlotLayout::observatoryHalfDepth;
constexpr float pi = 3.14159265358979323846f;

void boxAt(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);
    shapes::box(width, height, depth);
    glPopMatrix();
}

void drawWindow() {
    glColor3f(0.17f, 0.23f, 0.32f);
    shapes::box(1.74f, 1.68f, 0.22f);
    glColor3f(0.24f, 0.47f, 0.59f);
    boxAt(0.0f, 0.0f, 0.12f, 1.48f, 1.42f, 0.06f);
    glColor3f(0.76f, 0.71f, 0.58f);
    boxAt(0.0f, 0.0f, 0.16f, 0.065f, 1.42f, 0.06f);
    boxAt(0.0f, -0.86f, 0.17f, 1.85f, 0.12f, 0.34f);
}

void drawWindowRow(int floor) {
    glPushMatrix();
    glTranslatef(0.0f, 2.1f + 3.8f * floor, facadeZ + 0.08f);
    for (const float x : {-8.3f, -5.6f, -2.9f, 2.9f, 5.6f, 8.3f}) {
        glPushMatrix();
        glTranslatef(x, 0.0f, 0.0f);
        if (floor == 1) glScalef(1.0f, 0.87f, 1.0f);
        drawWindow();
        glPopMatrix();
    }
    glPopMatrix();
}

void drawEntrance() {
    glColor3f(0.57f, 0.58f, 0.59f);
    boxAt(0.0f, 0.11f, facadeZ + 0.93f, 5.55f, 0.22f, 1.96f);
    glColor3f(0.13f, 0.19f, 0.27f);
    boxAt(0.0f, 1.51f, facadeZ + 0.05f, 2.85f, 2.64f, 0.26f);
    glColor3f(0.23f, 0.44f, 0.55f);
    for (const float side : {-1.0f, 1.0f}) {
        boxAt(side * 0.65f, 1.59f, facadeZ + 0.2f, 1.18f, 2.19f, 0.1f);
    }

    glColor3f(0.22f, 0.29f, 0.39f);
    boxAt(0.0f, 3.5f, facadeZ + 0.85f, 5.58f, 0.52f, 1.92f);
    glColor3f(0.83f, 0.65f, 0.36f);
    boxAt(0.0f, 3.54f, facadeZ + 1.82f, 5.17f, 0.10f, 0.08f);

    for (const float side : {-1.0f, 1.0f}) {
        glColor3f(0.84f, 0.79f, 0.68f);
        boxAt(side * 2.46f, 1.75f, facadeZ + 1.46f, 0.28f, 3.18f, 0.32f);
        glColor3f(0.87f, 0.71f, 0.42f);
        boxAt(side * 0.18f, 1.22f, facadeZ + 0.28f, 0.05f, 0.27f, 0.06f);
    }
}

void drawBuilding() {
    glColor3f(0.61f, 0.63f, 0.63f);
    boxAt(0.0f, 0.26f, 0.0f, buildingWidth, 0.52f, buildingDepth);
    glColor3f(0.84f, 0.80f, 0.71f);
    boxAt(0.0f, 4.32f, 0.0f, buildingWidth - 0.4f, 7.6f, buildingDepth - 0.35f);
    glColor3f(0.46f, 0.50f, 0.52f);
    boxAt(0.0f, 4.06f, 0.0f, buildingWidth, 0.30f, buildingDepth);
    glColor3f(0.24f, 0.30f, 0.39f);
    boxAt(0.0f, 8.19f, 0.0f, buildingWidth + 0.5f, 0.56f, buildingDepth + 0.55f);

    for (int floor = 0; floor < 2; ++floor) {
        drawWindowRow(floor);
        glPushMatrix();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawWindowRow(floor);
        glPopMatrix();

        for (const float side : {-1.0f, 1.0f}) {
            for (const float z : {-5.4f, 0.0f, 5.4f}) {
                glPushMatrix();
                glTranslatef(side * PlotLayout::observatoryHalfWidth, 2.1f + 3.8f * floor, z);
                glRotatef(side * 90.0f, 0.0f, 1.0f, 0.0f);
                drawWindow();
                glPopMatrix();
            }
        }
    }

    drawEntrance();
}

constexpr float domeRadius = 7.85f;
constexpr float domeHeight = 6.7f;
constexpr float domeBase = 9.0f;

void domeVertex(float elevation, float azimuth) {
    const float x = std::cos(elevation) * std::cos(azimuth);
    const float y = std::sin(elevation);
    const float z = -std::cos(elevation) * std::sin(azimuth);
    glNormal3f(x / domeRadius, y / domeHeight, z / domeRadius);
    glVertex3f(domeRadius * x, domeHeight * y, domeRadius * z);
}

void drawDome() {
    constexpr int segments = 48;
    constexpr int bands = 8;
    constexpr float elevationStep = 0.5f * pi / bands;
    constexpr float azimuthStep = 2.0f * pi / segments;

    glPushMatrix();
    glTranslatef(0.0f, 8.47f, 0.0f);
    glColor3f(0.24f, 0.30f, 0.39f);
    shapes::cylinder(8.05f, 0.32f, segments);
    glTranslatef(0.0f, 0.32f, 0.0f);
    glColor3f(0.72f, 0.58f, 0.35f);
    shapes::cylinder(7.96f, domeBase - 8.79f, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, domeBase, 0.0f);
    glBegin(GL_QUADS);
    for (int segment = 0; segment < segments; ++segment) {
        const float a = segment * azimuthStep;
        const float b = (segment + 1) * azimuthStep;
        const float shade = segment % 4 == 0 ? 0.055f : 0.0f;
        glColor3f(0.28f + shade, 0.37f + shade, 0.49f + shade);
        for (int band = 0; band < bands - 1; ++band) {
            const float lower = band * elevationStep;
            const float upper = (band + 1) * elevationStep;
            domeVertex(lower, a);
            domeVertex(lower, b);
            domeVertex(upper, b);
            domeVertex(upper, a);
        }
    }
    glEnd();

    glColor3f(0.28f, 0.37f, 0.49f);
    glBegin(GL_TRIANGLES);
    for (int segment = 0; segment < segments; ++segment) {
        domeVertex((bands - 1) * elevationStep, segment * azimuthStep);
        domeVertex((bands - 1) * elevationStep, (segment + 1) * azimuthStep);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, domeHeight, 0.0f);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);
    for (int segment = 0; segment <= segments; ++segment) {
        const float angle = -segment * azimuthStep;
        glVertex3f(domeRadius * std::cos(angle), 0.0f, -domeRadius * std::sin(angle));
    }
    glEnd();
    glPopMatrix();
}

void drawTelescopeMount() {
    glPushMatrix();
    glTranslatef(0.0f, 14.75f, 0.0f);
    glColor3f(0.18f, 0.24f, 0.33f);
    shapes::cylinder(2.75f, 1.35f, 32);
    glColor3f(0.77f, 0.64f, 0.44f);
    glTranslatef(0.0f, 1.35f, 0.0f);
    shapes::cylinder(2.45f, 0.34f, 32);
    glPopMatrix();

    glColor3f(0.22f, 0.29f, 0.39f);
    for (const float side : {-1.0f, 1.0f}) {
        boxAt(side * 2.05f, 17.2f, 0.0f, 0.72f, 1.7f, 1.14f);
        glPushMatrix();
        glTranslatef(side * 2.39f, 17.78f, 0.0f);
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
    glRotatef(-62.0f, 0.0f, 1.0f, 0.0f);
    drawTelescopeMount();
    glTranslatef(0.0f, 17.78f, 0.0f);
    glRotatef(60.0f, 1.0f, 0.0f, 0.0f);
    drawOpticalTube();
    glPopMatrix();
}

}  // namespace

void drawObservatory() {
    drawBuilding();
    drawDome();
    drawTelescope();
}

}  // namespace team01
