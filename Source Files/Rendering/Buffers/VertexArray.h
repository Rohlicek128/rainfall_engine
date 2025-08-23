#pragma once
#include "VertexBuffer.h"
#include "Attributes/VertexAttribute.h"

class VertexArray : public IBindable
{
public:
    VertexArray(VertexBuffer*, const VertexAttribute*, int);
    ~VertexArray() override;
    void bind() override;
    void unbind() override;
};
