#pragma once
#include "../IBindable.h"
#include "glad/glad.h"

class Renderbuffer : public IBindable
{
    int width_, height_;
    GLenum format_;
    GLenum attachment_;
    
public:
    Renderbuffer(int, int, GLenum, GLenum);
    ~Renderbuffer() override = default;
    void resize(int, int);
    int get_width();
    int get_height();
    GLenum get_format();
    GLenum get_attachment();
    void bind() override;
    void unbind() override;
};
