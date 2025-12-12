#include "IndexBuffer.h"

#include <glad.h>

IndexBuffer::IndexBuffer(unsigned int data[], const unsigned int length)
{
    data_length = length;
    glGenBuffers(1, &handle_);

    IndexBuffer::bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, length * sizeof(unsigned int), data, GL_STATIC_DRAW);
    IndexBuffer::unbind();
}
IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &handle_);
}


void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle_);
}

void IndexBuffer::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
