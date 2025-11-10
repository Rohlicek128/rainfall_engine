#pragma once
#include <vector>

#include "VertexBuffer.h"
#include "Attributes/VertexAttribute.h"

class VertexArray : public IBindable
{
public:
    VertexArray(VertexBuffer*, const std::vector<VertexAttribute>&);
    ~VertexArray() override;
    void bind() override;
    void unbind() override;
};
