#pragma once

#include "imgui.h"
#include "window.h"
class UiContext_Imgaui
{
public:

    void init(Window window) ;

    void pre_render() ;

    void post_render() ;

    void end() ;

private:
};

