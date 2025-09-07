#include "PostProcessProgram.h"

#include <glad/glad.h>

#include "../../Buffers/Screen/Framebuffer.h"
#include "../../Entities/Components/CameraComponent.h"

PostProcessProgram::PostProcessProgram(const std::vector<Shader>& shaders, int width, int height) : Program(shaders)
{
    framebuffer_ = std::make_unique<Framebuffer>();
    framebuffer_->attach_texture_2d(std::make_unique<Texture>(width, height, GL_RGBA16F, GL_RGBA, GL_FLOAT), GL_COLOR_ATTACHMENT0);
    framebuffer_->attach_renderbuffer(std::make_unique<Renderbuffer>(width, height, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT));
    framebuffer_->set_draw_buffers();
    framebuffer_->check_completeness();
}

void PostProcessProgram::draw(Mesh& mesh, const int quad_index, const CameraComponent& camera_component, const int texture_handle)
{
    glDisable(GL_DEPTH_TEST);
    bind();
    mesh.bind();

    glActiveTexture(GL_TEXTURE0);
    if (texture_handle == -1) glBindTexture(GL_TEXTURE_2D, framebuffer_->attached_textures.at(0)->get_handle());
    else glBindTexture(GL_TEXTURE_2D, texture_handle);
    set_uniform("screen_texture", 0);
    
    set_uniform("gamma", camera_component.gamma);
    set_uniform("exposure", camera_component.exposure);

    if (const ModelData* model_data = mesh.get_model(quad_index))
    {
        glDrawElements(GL_TRIANGLES, model_data->indices_length,
            GL_UNSIGNED_INT, (void*)(mesh.get_model_indices_offset(quad_index) * sizeof(GLuint)));
    }

    mesh.unbind();
    unbind();
    glEnable(GL_DEPTH_TEST);
}

void PostProcessProgram::resize(const int width, const int height)
{
    framebuffer_->resize(width, height);
}

void PostProcessProgram::bind_framebuffer()
{
    framebuffer_->bind();
}

void PostProcessProgram::unbind_framebuffer()
{
    framebuffer_->unbind();
}
