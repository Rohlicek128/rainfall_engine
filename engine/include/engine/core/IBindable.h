#pragma once

class IBindable
{
protected:
    unsigned int handle_ = 0;
public:
    virtual ~IBindable() = default;
    unsigned int get_handle();
    virtual void bind() = 0;
    virtual void unbind() = 0;
};
