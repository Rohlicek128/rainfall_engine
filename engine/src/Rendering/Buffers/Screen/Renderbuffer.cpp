#include "engine/rendering/Renderbuffer.h"

#include <glad.h>


Renderbuffer::Renderbuffer(const int width, const int height, const unsigned int format, const unsigned int attachment)
{
    width_ = width;
    height_ = height;
    format_ = format;
    attachment_ = attachment;

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

GLenum Renderbuffer::get_format()
{
    return format_;
}

GLenum Renderbuffer::get_attachment()
{
    return attachment_;
}

void Renderbuffer::bind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, handle_);
}

void Renderbuffer::unbind()
{
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
