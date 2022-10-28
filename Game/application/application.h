#pragma once

#include <cstddef>
#include <string_view>
#include <memory>

#include "menu.h"
#include <game/render.h>

struct GLFWwindow;

namespace bugattack{class Map;}

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
    std::unique_ptr<class Render> render_;
    std::unique_ptr<bugattack::Map> map_;
};


