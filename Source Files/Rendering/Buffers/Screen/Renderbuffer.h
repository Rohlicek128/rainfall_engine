#pragma once
#include "../IBindable.h"
#include "glad/glad.h"

class Renderbuffer : public IBindable
{
    int width_, height_;
    GLenum format_;
    
public:
    Renderbuffer(int, int, GLenum);
    ~Renderbuffer() override = default;
    void resize(int, int);
    int get_width();
    int get_height();
    void bind() override;
    void unbind() override;
};
