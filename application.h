#pragma once

#include <cstddef>
#include <string_view>
#include <memory>

#include "map.h"
#include "menu.h"
#include "render.h"

struct GLFWwindow;

class Application
{
public:
    Application(std::string_view name, std::size_t width, std::size_t height);

    ~Application();

    void Run();
public:
    static void MouseEvent(GLFWwindow*, int, int, int);

    void ShowWinScreen();

    void ShowLostScreen();

    void Restart();

    std::size_t width_;
    std::size_t height_;

    GLFWwindow* window_;

    Menu menu_;
    std::unique_ptr<Render> render_;
    std::unique_ptr<Map> map_;
};
