#include "VertexArray.h"

#include "glad/glad.h"

VertexArray::VertexArray(VertexBuffer* vbo, const VertexAttribute* attributes, int length)
{
    glGenVertexArrays(1, &handle_);

    VertexArray::bind();
    vbo->bind();

    int stride = 0;
    for (int i = 0; i < length; ++i) stride += attributes[i].lenght;
    stride *= sizeof(float);

    int offset = 0;
    for (int i = 0; i < length; ++i)
    {
        glVertexAttribPointer(i, attributes[i].lenght, GL_FLOAT, GL_FALSE, stride, (void*)(offset * sizeof(float)));
        glEnableVertexAttribArray(i);
        offset += attributes[i].lenght;
    }
    
    vbo->unbind();
    VertexArray::unbind();
}
VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &handle_);
}


void VertexArray::bind()
{
    glBindVertexArray(handle_);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}
