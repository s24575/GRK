#ifndef TEXTUREMODEL_H
#define TEXTUREMODEL_H

#include <GL/glew.h>

class TextureModel{
public:
    void SetTextureUnit(GLuint t){texture_unit_=t;}
    void SetTexture(GLuint t){texture_ = t;}
    void SetTexturePrim(GLuint t){texture_prim_ = t;}
protected:
    GLuint texture_unit_;
    GLuint texture_;
    GLuint texture_prim_;
};

#endif // TEXTUREMODEL_H
