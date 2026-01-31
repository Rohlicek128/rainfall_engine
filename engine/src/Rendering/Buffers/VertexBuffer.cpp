#include "VertexBuffer.h"

#include <glad.h>


VertexBuffer::VertexBuffer(float data[], const unsigned int length)
{
    data_length = length;
    glGenBuffers(1, &handle_);

    VertexBuffer::bind();
    glBufferData(GL_ARRAY_BUFFER, length * sizeof(float), data, GL_STATIC_DRAW);
    VertexBuffer::unbind();
}
VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &handle_);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, handle_);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
