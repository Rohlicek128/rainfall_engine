#include "MeshComponent.h"

#include <iterator>

MeshComponent::MeshComponent(float verts[], unsigned int inds[], const int v_lenght, const int i_lenght, const GLenum type)
{
    vertices = verts;
    indices = inds;
    primitive_type = type;
    type_edit_ = type;
    
    verts_lenght = v_lenght;
    inds_lenght = i_lenght;
}

void MeshComponent::set_gui()
{
    if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Vertices: %i", verts_lenght);
        ImGui::Text("Indices: %i", inds_lenght);
        
        const char* type_names[] = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strip", "Triangle Fan", "Quads", "Quads Strip", "Polygon" };
        ImGui::Combo("Primitive Type", &type_edit_, type_names, std::size(type_names));
        primitive_type = type_edit_;
    }
}