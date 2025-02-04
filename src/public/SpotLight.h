﻿#pragma once
#include "PointLight.h"
class SpotLight:
    public PointLight
{
public:
    SpotLight();

    SpotLight(  GLfloat red, GLfloat green, GLfloat blue,
                GLfloat aIntensity, GLfloat dIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat xDir, GLfloat yDir, GLfloat zDir,
                GLfloat con, GLfloat lin, GLfloat exp,
                GLfloat edg);

    // NOTE: For location variables, it should be GLuint. Float works but is not... appropriate.
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);
    
    ~SpotLight();

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};
