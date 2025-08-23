#pragma once
#include "IBindable.h"

class VertexBuffer : public IBindable
{
    unsigned int handle_;
public:
    int data_length;
    
    VertexBuffer(float[], unsigned int);
    ~VertexBuffer() override;
    void bind() override;
    void unbind() override;
};
