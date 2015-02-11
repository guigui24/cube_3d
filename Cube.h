#ifndef DEF_CUBE
#define DEF_CUBE


// Includes OpenGL

#ifdef WIN32
#include <GL/glew.h>

#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>

#endif


// Includes GLM

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Includes

#include "Shader.h"


// Classe Cube

class Cube
{
    public:

    Cube(float taille, std::string const vertexShader, std::string const fragmentShader);
    ~Cube();

    void afficher(glm::mat4 &projection, glm::mat4 &modelview);
    glm::vec3 getTaille();
    void setPosition(glm::vec3 position);
    glm::vec3 getPosition();
    void setAngle(glm::vec3 angle);
    glm::vec3 getAngle();

    protected:

    glm::vec3 m_position;
    glm::vec3 m_angle;
    Shader m_shader;
    float m_vertices[108];
    float m_couleurs[108];
    float m_taille;
};

#endif
