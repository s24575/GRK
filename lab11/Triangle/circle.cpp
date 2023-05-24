#include "circle.h"

#include <GL/glew.h>
#include <cmath>

constexpr int vertex_count = 30;

void Circle::Initialize(){
    const int stride = 4;

    GLfloat kVertices[vertex_count * stride];
    GLfloat kColors[vertex_count * stride];

    const float radius = 0.5f;

    const float angle = 360.0f / vertex_count;

    for(int i = 0; i < vertex_count; i++){
        kVertices[stride * i] = std::sin(i * angle) * radius;
        kVertices[stride * i + 1] = std::cos(i * angle) * radius;
        kVertices[stride * i + 2] = 0.0f;
        kVertices[stride * i + 3] = 1.0f;

        kColors[stride * i] = 1.0f;
        kColors[stride * i + 1] = 1.0f;
        kColors[stride * i + 2] = 1.0f;
        kColors[stride * i + 3] = 1.0f;
    }

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kColors), kColors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Circle::~Circle(){
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &color_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao_);
}

void Circle::Draw(const Program &program){

    glUseProgram(program);
    glBindVertexArray(vao_);

    glDrawArrays(GL_LINE_LOOP, 0, vertex_count);

    glBindVertexArray(0);
    glUseProgram(0);


}
