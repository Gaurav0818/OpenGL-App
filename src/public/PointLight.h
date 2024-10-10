#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight();
    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity,
                GLfloat xPos, GLfloat yPos, GLfloat zPos,
                GLfloat con, GLfloat lin, GLfloat exp);

    // NOTE: For location variables, it should be GLuint. Float works but is not... appropriate.
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint diffuseIntensityLocation, GLuint positionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
protected:
    glm::vec3 position; // Where the point light is.

    GLfloat constant, linear, exponent; // Values controlling the attenuation of our light.
};

