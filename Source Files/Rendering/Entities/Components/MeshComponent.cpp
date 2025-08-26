#include "MeshComponent.h"

#include <iterator>

MeshComponent::MeshComponent(const int index, const GLenum type)
{
    model_index = index;
    
    primitive_type = type;
    type_edit_ = type;
}

void MeshComponent::set_gui()
{
    ImGui::Text("Model: %i", model_index);
    ImGui::SliderInt("Model", &model_index, 0, 5);
    //ImGui::Text("Vertices: %i", verts_lenght);
    //ImGui::Text("Indices: %i", inds_lenght);
        
    const char* type_names[] = { "Points", "Lines", "Line Loop", "Line Strip", "Triangles", "Triangle Strip", "Triangle Fan", "Quads", "Quads Strip", "Polygon" };
    ImGui::Combo("Primitive Type", &type_edit_, type_names, std::size(type_names));
    primitive_type = type_edit_;
}