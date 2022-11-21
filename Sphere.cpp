#include <iostream>
#include <fstream>
#include <map>
#include <tuple>
#include <numeric>
#include <vector>

#include "Sphere.h"

/* Code sampled from https://gist.github.com/zwzmzd/0195733fa1210346b00d, adjusted to use GL_QUADS instead of GL_QUADS_STRIP */
Sphere::Sphere( GLfloat radius , glm::vec3 color) {
    model = glm::mat4(1);
    this->radius = radius;
    scale(radius);
    //spin(90, glm::vec3(1.0f, 0.0f, 0.0f));
    this->color = color;

    int i, j;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
    std::vector<GLfloat> normals;
    int indicator = 0;
    for (i = 0; i <= stackCount; i++) {
        double lat0 = glm::pi<double>() * (-0.5 + (double)(i - 1) / stackCount);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = glm::pi<double>() * (-0.5 + (double)i / stackCount);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);
        for (j = 0; j <= sectorCount; j++) {
            double lng = 2 * glm::pi<double>() * (double)(j - 1) / sectorCount;
            double lng1 = 2 * glm::pi<double>() * (double)(j) / sectorCount;

            double x1 = cos(lng1);
            double y1 = sin(lng1);
            double x = cos(lng);
            double y = sin(lng);

            vertices.push_back(x * zr1);
            vertices.push_back(y * zr1);
            vertices.push_back(z1);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x * zr0);
            vertices.push_back(y * zr0);
            vertices.push_back(z0);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x1 * zr0);
            vertices.push_back(y1 * zr0);
            vertices.push_back(z0);
            indices.push_back(indicator);
            indicator++;

            vertices.push_back(x1 * zr1);
            vertices.push_back(y1 * zr1);
            vertices.push_back(z1);
            indices.push_back(indicator);
            indicator++;

            float x_avg = ((x * zr1) + (x * zr0) + (x1 * zr0) + (x1 * zr1)) / 4;
            float y_avg = ((y * zr1) + (y * zr0) + (y1 * zr0) + (y1 * zr1)) / 4;
            float z_avg = (2 * z0 + 2 * z1) / 4;

            glm::vec3 avg = glm::normalize(glm::vec3(x_avg, y_avg, z_avg));

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);

            normals.push_back(avg.x);
            normals.push_back(avg.y);
            normals.push_back(avg.z);
        }
        indices.push_back(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    }


    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);


    glGenBuffers(1, &vbo_n);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_n);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    numsToDraw = indices.size();
}

void Sphere::draw(const glm::mat4& projection, const glm::mat4& view, GLuint shader) {
    glUseProgram(shader);

    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glBindVertexArray(vao);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glDisable(GL_CULL_FACE);

    glPatchParameteri(GL_PATCH_VERTICES, 4);
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(GL_PRIMITIVE_RESTART_FIXED_INDEX);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glDrawElements(GL_QUADS, numsToDraw, GL_UNSIGNED_INT, NULL);

    glUseProgram(0);
    glBindVertexArray(0);
}

void Sphere::spin(float angle, glm::vec3 axis) {
    model = glm::rotate(glm::radians(angle), axis) * model;
}
void Sphere::update()
{
    this->spin(0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
}
void Sphere::scale(float level)
{
    model = glm::scale(glm::mat4(1.0f), glm::vec3(level)) * model;
}
void Sphere::translation(glm::vec3 destination)
{
    model = glm::translate(glm::mat4(1.0f), destination) * glm::scale(glm::mat4(1.0f), glm::vec3(radius));
}
void Sphere::changeColor(glm::vec3 color) 
{
    this->color = color;
}
