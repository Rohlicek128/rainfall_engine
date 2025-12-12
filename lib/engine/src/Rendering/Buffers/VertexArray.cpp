#include "VertexArray.h"

#include <glad.h>


VertexArray::VertexArray(VertexBuffer* vbo, const std::vector<VertexAttribute>& attributes)
{
    glGenVertexArrays(1, &handle_);

    VertexArray::bind();
    vbo->bind();

    int stride = 0;
    for (int i = 0; i < attributes.size(); ++i) stride += attributes[i].lenght;
    stride *= sizeof(float);

    int offset = 0;
    for (int i = 0; i < attributes.size(); ++i)
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
