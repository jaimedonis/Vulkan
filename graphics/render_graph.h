#pragma once

#include "mesh_render_object.h"
#include "../graphics/shader_program.h"
#include "../application/perspectivecamera.h"
#include "../entity_components/base_entity.h"
#include "../entity_components/transform_component.h"
#include "../entity_components/mesh_component.h"
#include "../graphics/mesh_render_object.h"

#include <memory>
#include <vector>


class RenderGraph
{
    typedef std::vector<std::unique_ptr<MeshRenderObject>> MeshRenderObjectsArray;

public:
    RenderGraph();

public:

    void OnLoad();

    void Render();

    void OnUnload();

private:

    std::shared_ptr<PerspectiveCamera> _camera;

    std::unique_ptr<BaseEntity> _root_entity;
    std::shared_ptr<TransformComponent> _transform_component;
    std::shared_ptr<MeshComponent> _mesh_component;

    std::unique_ptr<ShaderProgram> _shader_program;

    MeshRenderObjectsArray _opaque_objects;
};


