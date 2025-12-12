#pragma once
#include <memory>
#include <vector>

#include "Texture.h"

class Cubemap : public  IBindable
{
    void set_face(unsigned int, const std::string&);
public:
    std::unique_ptr<Texture> faces[6];
    GLenum internal_format, format, type;
    
    Cubemap(const std::vector<std::string>&, GLenum internal_format, GLenum format, GLenum type);
    ~Cubemap() override = default;

    void bind() override;
    void unbind() override;
};
