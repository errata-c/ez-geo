#include <ez/imgui/ImguiEngine.hpp>

#include <memory>
#include <vector>

#include <glm/vec2.hpp>
#include <fmt/format.h>
#include <ez/geo/BezierFunctions.hpp>
#include <ez/geo/BPath.hpp>
#include <RenderTools/RenderTools.hpp>
#include "imgui.h"
#include "nanovg.h"

class Engine : public ImGuiEngine {
public:
    Engine()
        : index(-1)
    {}

    virtual void postInitialize() override {
        glClearColor(1, 1, 1, 1);

        path.append(glm::vec2{ 0.2, 0.8 });
        path.append(glm::vec2{ 0.2, 0.2 });
        path.append(glm::vec2{ 0.4, 0.1 });
        path.append(glm::vec2{ 0.8, 0.2 });
        path.append(glm::vec2{ 0.8, 0.8 });

        fmt::print("The path has {} segments.\n", path.numSegments());
    }
    virtual void handleEvent(const ez::InputEvent& ev) override {
        if (index == -1) {
            if (ev.type == ez::InputEventType::MousePress && ev.mouse.isPressed(ez::Mouse::Left)) {
                glm::vec2 mpos = ev.mouse.position;
                glm::vec2 frame = getFrameSize();

                float dist2 = 8;
                std::ptrdiff_t select = -1;

                std::ptrdiff_t count = static_cast<std::ptrdiff_t>(path.size());
                for (std::ptrdiff_t i = 0; i < count; ++i) {
                    glm::vec2 cpos = path[i];
                    float dist1 = glm::length(cpos*frame - mpos);
                    if (dist1 < dist2) {
                        select = i;
                        dist2 = dist1;
                    }
                }

                index = select;
            }
        }
        else {
            if (ev.type == ez::InputEventType::MouseRelease && ev.mouse.isPressed(ez::Mouse::Left)) {
                index = -1;
            }
            else if (ev.type == ez::InputEventType::MouseMove) {
                glm::vec2 mpos = ev.mouse.position;
                glm::vec2 frame = getFrameSize();
                mpos /= frame;

                path[index] = mpos;
            }
        }
        
    }
    virtual void buildGui() override {
        
    }
    virtual void render() override {
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec2 window = getWindowSize();
        glm::vec2 frame = getFrameSize();

        NVGcontext* ctx = getNVGContext();
        nvgBeginFrame(ctx, window.x, window.y, window.x / frame.x);

        drawPath(frame);

        nvgEndFrame(ctx);

        glFlush();
    }

    void drawPath(glm::vec2 size) {
        NVGcontext* ctx = getNVGContext();

        nvgStrokeWidth(ctx, 3.f);
        
        nvgFillColor(ctx, nvgRGBf(0, 0, 1));
        for (auto& point : path) {
            glm::vec2 tmp = point * size;
            drawCircle(tmp, 5.f);
        }

        nvgStrokeColor(ctx, nvgRGBf(0, 0, 0));
        
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, path[0].x * size.x, path[0].y * size.y);

        std::size_t count = path.numSegments();
        for (std::size_t i = 0; i < count; ++i) {
            auto seg = path.segmentAt(i);
            for (auto& point : seg) {
                point = point * size;
            }

            nvgBezierTo(ctx, seg[1].x, seg[1].y, seg[2].x, seg[2].y, seg[3].x, seg[3].y);
        }
        nvgStroke(ctx);
    }
    void drawCubic(const std::array<glm::vec2, 4>& p) {
        NVGcontext* ctx = getNVGContext();
        nvgBeginPath(ctx);
        nvgMoveTo(ctx, p[0].x, p[0].y);
        nvgBezierTo(ctx, p[1].x, p[1].y, p[2].x, p[2].y, p[3].x, p[3].y);
        nvgStroke(ctx);
    }
    void drawCircle(glm::vec2 pos, float radius) {
        NVGcontext* ctx = getNVGContext();
        nvgBeginPath(ctx);
        nvgArc(ctx, pos.x, pos.y, radius, 0.f, ez::tau<float>(), NVG_CCW);
        nvgFill(ctx);
    }

    ez::BPath<float, 2> path;
    std::ptrdiff_t index;
};

int main(int argc, char ** argv) {
    Engine engine;
    if (!engine.initialize()) {
        return -1;
    }

    if (!engine.createWindow(800, 600, "QPath")) {
        return -1;
    }

    return engine.run();
}
