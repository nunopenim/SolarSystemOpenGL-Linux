#include <iostream>
#include "Shader.hh"
#include "global.h"
#include "camera.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <math.h>

# define M_PI 3.14159265358979323846 //jesus fuck paulo, era necessario?

class Ring {
public:
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<int> indices;
    unsigned int cubeVAO, cubeVBO, cubeEBO, texturePlanet;

    glm::vec3 modelVec = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 moveTo = glm::vec3(0.0f, 0.0f, 0.0f);

    float orbitS = 0.0f;
    float rotationS = 0.0f;
    float secondOrbit = 0.0f;
    float angle = 0.0f;
    glm::vec3 secondModelVec = glm::vec3(0.0f, 0.0f, 0.0f);

    Shader shader;

    Ring(float radius, int sectorCount, int stackCount, const char * textureName) {

        // clear memory of prev arrays
        float PI = M_PI;

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
        float s, t;                                     // vertex texCoord

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            for (int i = 0; i <= stackCount; ++i)
            {
                stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
                xy = radius * cosf(stackAngle);             // r * cos(u)
                z = 0;

                // add (sectorCount+1) vertices per stack
                // the first and last vertices have same position and normal, but different tex coords
                for (int j = 0; j <= sectorCount; ++j)
                {
                    sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                    // vertex position (x, y, z)
                    x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                    y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)

                    if (x > 0.25 && y > 0.25 && x < 0.75 && y < 0.75) {
                        continue;
                    }

                    vertices.push_back(x);
                    vertices.push_back(y);
                    vertices.push_back(z);

                    // normalized vertex normal (nx, ny, nz)
                    nx = x * lengthInv;
                    ny = y * lengthInv;
                    nz = z * lengthInv;
                    normals.push_back(nx);
                    normals.push_back(ny);
                    normals.push_back(nz);

                    // vertex tex coord (s, t) range between [0, 1]
                    s = (float)j / sectorCount;
                    t = (float)i / stackCount;
                    texCoords.push_back(s);
                    texCoords.push_back(t);
                }
            }
        }
        // generate CCW index list of sphere triangles

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                // k1+1 => k2 => k2+1
                if (i != (stackCount- 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }
            }
        }

        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);

        glBindVertexArray(cubeVAO);

        GLfloat objectVerts[9] = {
            -0.5f, -0.5f, -0.5f, // front bottom left  0
            0.5f, -0.5f, -0.5f, // front bottom right 1
            0.5f,  0.5f, -0.5f, // front top right    2
        };
        float* pvertarray = &vertices[0];
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

        //int *indicesarray = &indices[0];
        GLuint indexArr[3] = {
            0, 1, 2, // front face
        };
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        unsigned int cubeVBO2;
        glGenBuffers(1, &cubeVBO2);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);

        //    glBindBuffer(GL_ARRAY_BUFFER, 0);
        //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        unsigned int tex;
        glGenBuffers(1, &tex);
        glBindBuffer(GL_ARRAY_BUFFER, tex);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0])* texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);

        glGenTextures(1, &texturePlanet);
        glBindTexture(GL_TEXTURE_2D, texturePlanet);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        unsigned char* data = stbi_load(textureName, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

    };

    void useShader(const char* vertexPath, const char* fragmentPath) {
        Shader shader1(vertexPath, fragmentPath);
        shader = shader1;
    };
    void shift(float x, float y, float z) {
        this->moveTo = glm::vec3(x, y, z);
    }

    void rotationSpeed(float speed) {
        this->rotationS = speed;
    }

    void rotate(float angle) {
        this->angle = angle;
    }
    void orbitSpeed(float speed) {
        this->orbitS = speed;
    }

    void secondRotate(float x, float y, float z) {
        this->secondModelVec = glm::vec3(x, y, z);
    }
    void secondOrbitSpeed(float speed) {
        this->secondOrbit = speed;
    }

    void update() {

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //this-> modelVec = glm::vec3(0.0f, 1.0f, 0.0f);
        //this-> moveTo = glm::vec3(0.0f, 0.0f, 5.0f);

        if (secondOrbit != 0) {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(orbitS), glm::vec3(0.0f, 1.0f, 0.0f))//anos sol
                * glm::translate(model, this->moveTo)//distancia ao sol
                * glm::rotate(model, (float)glfwGetTime() * glm::radians(secondOrbit - orbitS), glm::vec3(0.0f, 1.0f, 0.0f)) //anos planeta principal
                * glm::translate(model, this->secondModelVec) //distancia ao planeta principal
                * glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f)) //angulo
                * glm::rotate(model, (float)glfwGetTime() * glm::radians(rotationS - secondOrbit), glm::vec3(0.0f, 1.0f, 0.0f)); //dias
        }
        else {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(orbitS), glm::vec3(0.0f, 1.0f, 0.0f)) //anos
                * glm::translate(model, this->moveTo) //distancia ao sol
                * glm::rotate(model, (float)glfwGetTime() * glm::radians(-orbitS), glm::vec3(0.0f, 1.0f, 0.0f)) //estações
                * glm::rotate(model, glm::radians(angle), glm::vec3(-1.0f, 0.0f, 0.0f)) // angulo
                * glm::rotate(model, (float)glfwGetTime() * glm::radians(rotationS), glm::vec3(0.0f, 1.0f, 0.0f)); //dias
        }

        view = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);

        shader.update(model, view, projection);

        glDrawElements(GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_INT,
            (void*)0);
    }

    void draw() {
        shader.use();
        glBindTexture(GL_TEXTURE_2D, texturePlanet);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // when texture area is large, bilinear filter the original
        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);
        update();
    }
};