#include "application.h"

#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <game/map.h>
#include <game/game_render.h>


Application::Application(std::string_view name, std::size_t width, std::size_t height) :
    width_{ width }, height_{ height }, window_{},
    menu_{}
{
    if(glfwInit() != GLFW_TRUE)
    {
        std::cerr << "GLFW initiation failed" << std::endl;
        std::terminate();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window_ = glfwCreateWindow(width_, height_, name.data(), nullptr, nullptr);

    if(window_ == nullptr)
    {
        const char* description;
        glfwGetError(&description);

        std::cerr << description << std::endl;
        std::terminate();
    }

    glfwMakeContextCurrent(window_);

    glfwSetWindowUserPointer(window_, this);

    glfwSetMouseButtonCallback(window_, Application::MouseEvent);

    glewExperimental = GL_TRUE;

    GLenum error_code = glewInit();
    if (error_code != GLEW_OK)
    {
        std::cerr << glewGetErrorString(error_code) << std::endl;
        std::terminate();
    }

    glViewport(0, 0, width_, height_);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    render_ = std::make_unique<class Render>(width, height);

    menu_.AddButton(Render::LoadTexture("../textures/Gun.rgba", 64, 64), 640.0f, 40.0f, 50.0f, 50.0f);
    menu_.AddButton(Render::LoadTexture("../textures/Laser.rgba", 64, 64), 700.0f, 40.0f, 50.0f, 50.0f);
    menu_.AddButton(Render::LoadTexture("../textures/RocketGun.rgba", 64, 64), 640.0f, 100.0f, 50.0f, 50.0f);

    map_ = std::make_unique<bugattack::Map>();

    render_->AddUnitTexture(Render::LoadTexture("../textures/Map.rgba", 1024, 1024), bugattack::UnitType::Map, 620.0f, 620.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Bug_1.rgba", 64, 64), bugattack::UnitType::Bug1, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Bug_2.rgba", 64, 64), bugattack::UnitType::Bug2, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Bug_3.rgba", 64, 64), bugattack::UnitType::Bug3, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Gun.rgba", 64, 64), bugattack::UnitType::Gun, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Laser.rgba", 64, 64), bugattack::UnitType::Laser, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/RocketGun.rgba", 64, 64), bugattack::UnitType::RocketGun, 40.0f, 40.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Rocket.rgba", 64, 64), bugattack::UnitType::Rocket, 40.0f, 30.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Shot.rgba", 64, 64), bugattack::UnitType::Shot, 5.0f, 5.0f);
    render_->AddUnitTexture(Render::LoadTexture("../textures/Boom.rgba", 128, 128), bugattack::UnitType::Boom, 1.0f, 1.0f);
}

Application::~Application()
{
    glfwTerminate();
}

void Application::Run()
{
    auto radius_texture = Render::LoadTexture("../textures/Radius.rgba", 1024, 1024);

    float previous = glfwGetTime();

    while (!glfwWindowShouldClose(window_))
    {
        glfwPollEvents();

        float now = glfwGetTime();
        float delta = now - previous;
        previous = now;

        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);

        menu_.MouseHover(xpos, ypos);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        map_->Update(delta);

        if(map_->IsWin())
        {
            ShowWinScreen();
            glfwSwapBuffers(window_);
            continue;
        }

        if(map_->IsLost())
        {
            ShowLostScreen();
            glfwSwapBuffers(window_);
            continue;
        }

        menu_.Render(*render_);

        bugattack::Render(*render_, *map_);

        auto xpos_ = glm::clamp<int>(glm::round(xpos/20)*20, 20, 600);
        auto ypos_ = glm::clamp<int>(glm::round(ypos/20)*20, 20, 600);

        auto selected = menu_.Selected();
        if(selected)
        {
            glm::vec4 color;
            if(map_->CanPut(glm::vec2{xpos_, ypos_}/20.0f))
                color = {0.6f, 1.0f, 0.2f, 0.4f};
            else
                color = {1.0f, 0.0f, 0.0f, 0.4f};
            auto radius = 0;
            if(selected == 1)
                radius = 100;
            else if (selected == 2)
                radius = 150;
            else radius = 250;

            render_->DrawTexture(radius_texture, xpos_, ypos_, 2*radius, 2*radius, 0.0f, 0.5f);

            render_->DrawRectangle(xpos_, ypos_, 40.0f, 40.0f, color);

        }

        glfwSwapBuffers(window_);
    }
}

void Application::MouseEvent(GLFWwindow* window, int button, int action, int mode)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        app->menu_.MouseClick(xpos, ypos);

        if(app->map_->IsWin() || app->map_->IsLost())
        {
            app->Restart();
            return;
        }

        if(0 < xpos && xpos < 620 && 0 < ypos && ypos < 620)
        {
            double xpos_ = glm::clamp<int>(glm::round(xpos/20), 1, 31);
            double ypos_ = glm::clamp<int>(glm::round(ypos/20), 1, 31);

            auto selected = app->menu_.Selected();

            if(selected == 0)
                return;
            if(selected == 1)
            {
                app->map_->Put(bugattack::UnitType::Gun, xpos_, ypos_);
            }
            if(selected == 2)
            {
                app->map_->Put(bugattack::UnitType::Laser, xpos_, ypos_);
            }
            if(selected == 3)
            {
                app->map_->Put(bugattack::UnitType::RocketGun, xpos_, ypos_);
            }
        }
    }
}

void Application::ShowWinScreen()
{
    render_->DrawText("You Win!", 300.0f, 320.0f, {0.5, 0.8f, 0.2f, 1.0f}, 3.0f);
}

void Application::ShowLostScreen()
{
    render_->DrawText("You Lost!", 300.0f, 320.0f, {0.5, 0.8f, 0.2f, 1.0f}, 3.0f);
}

void Application::Restart()
{
    map_->Restart();
}
