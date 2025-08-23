#pragma once
#include "IBindable.h"

class IndexBuffer : public IBindable
{
public:
    unsigned int data_length;
    
    IndexBuffer(unsigned int[],unsigned int);
    ~IndexBuffer() override;
    void bind() override;
    void unbind() override;
};
