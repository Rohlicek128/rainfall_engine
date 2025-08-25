#pragma once
#include "../IBindable.h"
#include "glad/glad.h"

class Renderbuffer : IBindable
{
    int width_, height_;
    GLenum format_;
    
public:
    Renderbuffer(int, int, GLenum);
    ~Renderbuffer() override;
    unsigned int get_handle();
    void resize(int, int);
    void bind() override;
    void unbind() override;
};
