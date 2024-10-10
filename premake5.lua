workspace "OpenGL-Application"
    architecture "x64"
    startproject "OpenGL-Application"

    configurations
    {
        "Debug",
        "Release"
    }

outputDir = "%{cfg.buildfg}-%{cfg.system}"

project "OpenGL-Application"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"

    targetdir ("temp/bin/%{cfg.buildfg}")
    objdir ("temp/obj/%{cfg.buildfg}")

    files
    {
        "src/public/**.h",
        "src/private/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src/public/",
        "vendor/include",
        "vendor/include/gl",
        "vendor/include/glfw",
        "vendor/include/assimp"
    }

    libdirs
    {
        "vendor/libs/gl",
        "vendor/libs/glfw",
        "vendor/libs/assimp"
    }

    links
    {
        "opengl32",
        "glfw3",
        "glew32",
        "assimp-vc143-mtd"
    }