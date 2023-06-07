//#include <GL/glew.h>
#include "plane.h"
#include "vertices.h"

void Plane::Initialize(int m, int n){
    m_=m;
    n_=n;

    //model_matrix_.Translate(0, -4, 0);
    model_matrix_.Translate(0, -40, 0);
    model_matrix_.RotateAboutX(80);

    TextureVertex vertices[4]={

        {{-(float)(m),0.0f,-(float)(n),1.0f},{(float)(0),(float)(1)}},
        {{(float)(m),0.0f,-(float)(n),1.0f},{(float)(1),(float)(1)}},
        {{(float)(m),0.0f,(float)(n),1.0f},{(float)(1),(float)(0)}},
        {{-(float)(m),0.0f,(float)(n),1.0f},{(float)(0),(float)(0)}}


    };

    GLuint indices[4]={ 0, 1, 3, 2};

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (GLvoid*)sizeof(vertices[0].position));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindVertexArray(0);

}



void Plane::Draw(const TextureCameraProgram &program) const{

    glBindVertexArray(vao_);


    glUseProgram(program);
    program.SetModelMatrix(model_matrix_);


    glActiveTexture(texture_unit_);
    glBindTexture(GL_TEXTURE_2D, texture_);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (GLvoid*)0);
    glDisable(GL_CULL_FACE);

    glBindVertexArray(0);
    glUseProgram(0);
}