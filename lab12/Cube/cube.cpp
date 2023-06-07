#include <GL/glew.h>
#include <iostream>

#include "cube.h"
#include "vertices.h"

Cube::Cube(float init_velocity, float init_angle){
    velocity_ = init_velocity;
    angle_ = init_angle;
    animated_ = true;
}


void Cube::Move(float delta_t){
    if (!animated_) return;
    angle_ += delta_t * velocity_;
    if(angle_>360) angle_ -= 360;
    if(angle_<-360) angle_ += 360;
    model_matrix_.SetUnitMatrix();
    model_matrix_.RotateAboutX(angle_);
    model_matrix_.RotateAboutY(angle_);
}



void Cube::SpeedUp(){
    velocity_ *= 1.09544511501;
}

void Cube::SlowDown(){
    velocity_ /= 1.09544511501;
}

void Cube::ToggleAnimated(){
    animated_ = ! animated_;
}

void Cube::Left(){
    model_matrix_.RotateAboutY(-3);
}

void Cube::Right(){
    model_matrix_.RotateAboutY(3);
}

void Cube::Up(){
    model_matrix_.RotateAboutX(-3);
}

void Cube::Down(){
    model_matrix_.RotateAboutX(3);
}

void Cube::Initialize(){
//    const ColorVertex kVertices[8] =
//    {
//        { { -.5f, -.5f,  .5f, 1.0f }, { 1, 1, 1, 1 } },
//        { { -.5f,  .5f,  .5f, 1.0f }, { 1, 0, 0, 1 } },
//        { {  .5f,  .5f,  .5f, 1.0f }, { 0, 1, 0, 1 } },
//        { {  .5f, -.5f,  .5f, 1.0f }, { 1, 1, 0, 1 } },
//        { { -.5f, -.5f, -.5f, 1.0f }, { 0, 0, 1, 1 } },
//        { { -.5f,  .5f, -.5f, 1.0f }, { 1, 0, 0, 1 } },
//        { {  .5f,  .5f, -.5f, 1.0f }, { 1, 0, 1, 1 } },
//        { {  .5f, -.5f, -.5f, 1.0f }, { 0, 0, 0, 1 } }
//    };

    const ColorVertex kVertices[] = {
        { { -1.0f, -1.0f,  1.0f, 1.0f }, { 1, 1, 1, 1 } },
        { {  0.0f, -1.0f,  1.0f, 1.0f }, { 1, 0, 0, 1 } },
        { {  1.0f, -1.0f,  1.0f, 1.0f }, { 0, 1, 0, 1 } },
        { {  1.0f, -1.0f, -1.0f, 1.0f }, { 1, 1, 0, 1 } },
        { {  1.0f,  0.0f, -1.0f, 1.0f }, { 0, 0, 1, 1 } },
        { {  0.0f,  1.0f, -1.0f, 1.0f }, { 1, 0, 0, 1 } },
        { {  1.0f,  0.0f,  0.0f, 1.0f }, { 1, 1, 1, 1 } },
        { { -1.0f,  0.0f, -1.0f, 1.0f }, { 1, 0, 0, 1 } },
        { {  1.0f,  0.0f,  1.0f, 1.0f }, { 1, 0, 1, 1 } },
        { { -1.0f, -1.0f, -1.0f, 1.0f }, { 1, 0, 1, 1 } },
        { { -1.0f,  0.0f,  0.0f, 1.0f }, { 0, 1, 0, 1 } },
        { { -1.0f,  0.0f,  1.0f, 1.0f }, { 0, 1, 1, 1 } }
    };

    const GLuint kIndices[] =
    {
        0, 1, 11,
        1, 2, 8,
        0, 1, 9,
        1, 3, 9,
        1, 2, 3,
        2, 3, 6,
        2, 6, 8,
        3, 4, 6,
        9, 3, 4,
        9, 4, 7,
        4, 5, 7,
        0, 9, 10,
        9, 7, 10,
        0, 10, 11,
        1, 10, 11,
        1, 6, 8,
        1, 6, 10,
        10, 6, 5,
        5, 4, 6,
        5, 7, 10
    };


    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(kVertices), kVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]), (GLvoid*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(kVertices[0]),
                          (GLvoid*) sizeof(kVertices[0].position));
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(kIndices), kIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Cube::Draw(const ModelProgram &program) const{

    glUseProgram(program);
    glBindVertexArray(vao_);

    program.SetModelMatrix(model_matrix_);

    glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glUseProgram(0);


}
