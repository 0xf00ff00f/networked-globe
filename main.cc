#include "panic.h"

#include "geometry.h"
#include "rand.h"
#include "shader_program.h"
#include "pixmap.h"
#include "util.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <iostream>
#include <memory>

class globe_geometry
{
public:
    globe_geometry(float radius, int max_subdivisions, const pixmap &map)
        : radius_(radius)
        , max_subdivisions_(max_subdivisions)
    {
        initialize_geometry(map);
        std::cout << (verts_.size() / 3) << " triangles\n";
    }

    void render() const
    {
        geometry_.bind();
        glDrawArrays(GL_TRIANGLES, 0, verts_.size());
    }

private:
    void initialize_geometry(const pixmap &map)
    {
        static const glm::vec3 icosahedron_verts[] = {
            {0, -0.525731, 0.850651},  {0.850651, 0, 0.525731},   {0.850651, 0, -0.525731}, {-0.850651, 0, -0.525731},
            {-0.850651, 0, 0.525731},  {-0.525731, 0.850651, 0},  {0.525731, 0.850651, 0},  {0.525731, -0.850651, 0},
            {-0.525731, -0.850651, 0}, {0, -0.525731, -0.850651}, {0, 0.525731, -0.850651}, {0, 0.525731, 0.850651}};
        static const int icosahedron_tris[][3] = {{2, 3, 7},  {2, 8, 3},   {4, 5, 6},   {5, 4, 9},  {7, 6, 12},
                                                  {6, 7, 11}, {10, 11, 3}, {11, 10, 4}, {8, 9, 10}, {9, 8, 1},
                                                  {12, 1, 2}, {1, 12, 5},  {7, 3, 11},  {2, 7, 12}, {4, 6, 11},
                                                  {6, 5, 12}, {3, 8, 10},  {8, 2, 1},   {4, 10, 9}, {5, 9, 1}};

        for (const auto &triangle : icosahedron_tris)
        {
            const auto &v0 = icosahedron_verts[triangle[0] - 1];
            const auto &v1 = icosahedron_verts[triangle[1] - 1];
            const auto &v2 = icosahedron_verts[triangle[2] - 1];
            subdivide_triangle(v0, v1, v2, 0, map);
        }

        geometry_.set_data(verts_);
    }

    void subdivide_triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, int level, const pixmap &map)
    {
        if (level == max_subdivisions_)
        {
            maybe_emit_triangle(v0, v1, v2, map);
        }
        else
        {
            const auto v01 = radius_ * glm::normalize(0.5f * (v0 + v1));
            const auto v12 = radius_ * glm::normalize(0.5f * (v1 + v2));
            const auto v20 = radius_ * glm::normalize(0.5f * (v2 + v0));

            subdivide_triangle(v0, v01, v20, level + 1, map);
            subdivide_triangle(v01, v1, v12, level + 1, map);
            subdivide_triangle(v20, v12, v2, level + 1, map);
            subdivide_triangle(v01, v12, v20, level + 1, map);
        }
    }

    void maybe_emit_triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2, const pixmap &map)
    {
        const auto center = glm::normalize((1.0f / 3) * (v0 + v1 + v2));

        const float longitude = (std::atan2(center.z, -center.x) + M_PI) / (2.0 * M_PI);

        const float d = sqrtf(center.x * center.x + center.z * center.z);
        const float latitude = (std::atan2(-center.y, d) + 0.5 * M_PI) / M_PI;

        const int c = static_cast<int>(longitude * map.width);
        const int r = static_cast<int>(latitude * map.height);

        if (!map.data[r * map.width + c])
            return;

        static constexpr float scale = 0.5f;
        const auto va = center + scale * (v0 - center);
        const auto vb = center + scale * (v1 - center);
        const auto vc = center + scale * (v2 - center);

        verts_.push_back(va);
        verts_.push_back(vb);
        verts_.push_back(vc);
    }

    float radius_;
    int max_subdivisions_;
    using vertex = std::tuple<glm::vec3>;
    std::vector<vertex> verts_;
    geometry geometry_;
};

class demo
{
public:
    demo(int window_width, int window_height)
        : window_width_(window_width)
        , window_height_(window_height)
    {
        initialize_meshes();
        initialize_shader();
    }

    void render_and_step(float dt)
    {
        render(program_);
        cur_time_ += dt;
    }

private:
    void initialize_meshes()
    {
        auto map = load_pixmap_from_png("assets/map.png");
        if (map->type != pixmap::pixel_type::GRAY)
            panic("eh?");
        globe_.reset(new globe_geometry(1, 5, *map));
    }

    void initialize_shader()
    {
        program_.add_shader(GL_VERTEX_SHADER, "shaders/sphere.vert");
        program_.add_shader(GL_FRAGMENT_SHADER, "shaders/sphere.frag");
        program_.link();
    }

    void render(const shader_program &program) const
    {
        glViewport(0, 0, window_width_, window_height_);
        glClearColor(0.5, 0.5, 0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_CULL_FACE);

        const auto projection =
            glm::perspective(glm::radians(45.0f), static_cast<float>(window_width_) / window_height_, 0.1f, 100.f);
        const auto view_pos = glm::vec3(0, 1.3, 3);
        const auto view_up = glm::vec3(0, 1, 0);
        const auto view = glm::lookAt(view_pos, glm::vec3(0, 0, 0), view_up);

        const auto angle = 0.75f * cur_time_;
        const auto model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0));
        const auto mvp = projection * view * model;

        glm::mat3 model_normal(model);
        model_normal = glm::inverse(model_normal);
        model_normal = glm::transpose(model_normal);

        program_.bind();
        program_.set_uniform(program.uniform_location("mvp"), mvp);

        program_.set_uniform(program.uniform_location("color"), glm::vec4(1));
        glCullFace(GL_BACK);
        globe_->render();

        program_.set_uniform(program.uniform_location("color"), glm::vec4(0.8));
        glCullFace(GL_FRONT);
        globe_->render();
    }

    int window_width_;
    int window_height_;
    float cur_time_ = 0;
    shader_program program_;
    std::unique_ptr<globe_geometry> globe_;
};

int main()
{
    constexpr auto window_width = 800;
    constexpr auto window_height = 400;

    if (!glfwInit())
        panic("glfwInit failed\n");

    glfwSetErrorCallback([](int error, const char *description) { panic("GLFW error: %s\n", description); });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 16);
    auto *window = glfwCreateWindow(window_width, window_height, "demo", nullptr, nullptr);
    if (!window)
        panic("glfwCreateWindow failed\n");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glewInit();

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mode) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
    });

#ifdef DUMP_FRAMES
    constexpr auto total_frames = 3 * 40;
    auto frame_num = 0;
#endif

    {
        demo d(window_width, window_height);

        while (!glfwWindowShouldClose(window))
        {
#ifdef DUMP_FRAMES
            const auto dt = 1.0f / 40;
#else
            const auto dt = 1.0f / 60;
#endif
            d.render_and_step(dt);

#ifdef DUMP_FRAMES
            char path[80];
            std::sprintf(path, "%05d.ppm", frame_num);
            dump_frame_to_file(path, window_width, window_height);

            if (++frame_num == total_frames)
                break;
#endif

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}
