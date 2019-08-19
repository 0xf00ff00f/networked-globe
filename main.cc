#include "panic.h"

#include "geometry.h"
#include "rand.h"
#include "shader_program.h"
#include "globe_geometry.h"
#include "cities_geometry.h"
#include "util.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <memory>

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
        render();
        cur_time_ += dt;
    }

private:
    void initialize_meshes()
    {
        globe_ = build_globe_geometry(6, "assets/map.png");
        cities_ = build_cities_geometry();
    }

    void initialize_shader()
    {
        globe_program_.add_shader(GL_VERTEX_SHADER, "shaders/sphere.vert");
        globe_program_.add_shader(GL_FRAGMENT_SHADER, "shaders/sphere.frag");
        globe_program_.link();

        cities_program_.add_shader(GL_VERTEX_SHADER, "shaders/cities.vert");
        cities_program_.add_shader(GL_GEOMETRY_SHADER, "shaders/cities.geom");
        cities_program_.add_shader(GL_FRAGMENT_SHADER, "shaders/cities.frag");
        cities_program_.link();
    }

    void render() const
    {
        glViewport(0, 0, window_width_, window_height_);
        glClearColor(0.5, 0.5, 0.5, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_MULTISAMPLE);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

        glEnable(GL_CULL_FACE);
        globe_program_.bind();
        globe_program_.set_uniform(globe_program_.uniform_location("mvp"), mvp);
        globe_program_.set_uniform(globe_program_.uniform_location("color"), glm::vec4(1));
        glCullFace(GL_BACK);

        globe_->render(GL_TRIANGLES);
        globe_program_.set_uniform(globe_program_.uniform_location("color"), glm::vec4(0.8));
        glCullFace(GL_FRONT);
        globe_->render(GL_TRIANGLES);

        glDepthMask(GL_FALSE);
        glDisable(GL_CULL_FACE);
        cities_program_.bind();
        cities_program_.set_uniform(cities_program_.uniform_location("mvp"), mvp);
        cities_->render(GL_POINTS);
        glDepthMask(GL_TRUE);
    }

    int window_width_;
    int window_height_;
    float cur_time_ = 0;
    std::unique_ptr<geometry> globe_;
    std::unique_ptr<geometry> cities_;
    shader_program globe_program_;
    shader_program cities_program_;
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
