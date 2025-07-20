#pragma once

#include "render_object.h"
#include "../graphics_resources/mesh.h"
#include "../graphics_resources/surface_material.h"

#include <memory>

// TODO: rename to MeshRenderInstance or similar...
class MeshRenderObject : public RenderObject
{
public:
    MeshRenderObject();

public:
    void OnLoad() override;

    void OnUnload() override;

    void OnRender() override;

public:
    Mesh* _mesh_resource = nullptr; // TODO: use shared_ptr
    std::shared_ptr<SurfaceMaterial> _material_resource; // TODO: add submeshes, assign one material for each one.

private:
    uint32_t _vertex_array_object = 0;
    uint32_t _vertex_position_object = 0;
    uint32_t _vertex_index_buffer_object = 0;
    uint32_t _vertex_normals_buffer_object = 0;
    uint32_t _vertex_uv_buffer_object = 0;
    uint32_t _vertex_colors_buffer_object = 0;
    uint32_t _vertex_tangents_buffer_object = 0;
    uint32_t _vertex_bitangents_buffer_object = 0;
};


