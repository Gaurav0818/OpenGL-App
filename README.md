# OpenGL-Project

## Overview
This is an OpenGL-based application that demonstrates various graphics techniques, including lighting, texturing, and model loading. The application is built using C++ and several libraries such as GLFW, GLEW, and Assimp.

## Features
- **Camera Control**: Move the camera using `W`, `A`, `S`, `D` keys and control the view direction with the mouse.
- **Lighting**: Supports directional, point, and spotlights.
- **Texturing**: Load and apply textures to models.
- **Model Loading**: Load 3D models using Assimp.

## Dependencies
- **GLFW**: For window creation and input handling.
- **GLEW**: For managing OpenGL extensions.
- **Assimp**: For loading 3D models.
- **GLM**: For mathematics operations.

## Setup
1. **Generate project files**:
    Run the `GenerateProject.bat` script to generate Visual Studio project files.
    ```sh
    ./GenerateProject.bat
    ```

2. **Build the project**:
    Open the generated Visual Studio solution file and build the project.

## Usage
- **Run the application**:
    Execute the built application from the Visual Studio environment or directly from the output directory.

- **Controls**:
    - `W`, `A`, `S`, `D`: Move the camera.
    - Mouse: Control the view direction.
    - `ESC`: Exit the application.

## Directory Structure
- `src/`: Contains the source code.
- `vendor/`: Contains third-party libraries.
- `temp/`: Output directory for build files.

## Acknowledgements
- This project uses the [GLFW](https://www.glfw.org/), [GLEW](http://glew.sourceforge.net/), and [Assimp](http://www.assimp.org/) libraries.