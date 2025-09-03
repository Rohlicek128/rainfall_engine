#include "Renderbuffer.h"

Renderbuffer::Renderbuffer(const int width, const int height, const GLenum format)
{
    width_ = width;
    height_ = height;
    format_ = format;
    
    glGenRenderbuffers(1, &handle_);
    Renderbuffer::bind();
    glRenderbufferStorage(GL_RENDERBUFFER, format_, width_, height_);
    Renderbuffer::unbind();
}

void Renderbuffer::resize(const int width, const int height)
{
    width_ = width;
    height_ = height;
    
    glDeleteRenderbuffers(1, &handle_);
    glGenRenderbuffers(1, &handle_);
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, format_, width_, height_);
    unbind();
}

int Renderbuffer::get_width()
{
    return width_;
}

int Renderbuffer::get_height()
{
    return height_;
}

void Renderbuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, handle_);
}

void Renderbuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
