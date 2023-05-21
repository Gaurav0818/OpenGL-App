#include "UiContext_Imgaui.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"


void UiContext_Imgaui::init(Window window)
{
    // Initialize ImGui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO();
    // Enable multi-viewport support
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigViewportsNoTaskBarIcon = true;

    // Initialize ImGui GLFW backend
    //ImGui_ImplGlfw_InitForOpenGL(window.GetGlfwWindow(), true);

    // Initialize ImGui OpenGL backend
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UiContext_Imgaui::pre_render()
{

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    // Create a main viewport
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::Begin("Main Viewport", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove);
    ImGui::End();

    // Render your GLFW window within the ImGui viewport
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::Begin("GLFW Window");
    // Render your GLFW content here
    ImGui::Text("Hello, GLFW Window!");
    ImGui::End();

    // Render ImGui
    ImGui::Render();

}

void UiContext_Imgaui::post_render()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();  // Call UpdatePlatformWindows() after Render() and before swapping buffers
}

void UiContext_Imgaui::end()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
