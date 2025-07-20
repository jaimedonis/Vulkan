#pragma once
#include "mesh_render_object.h"
#include "../application/perspectivecamera.h" // TODO: use abstract class
#include "../graphics/shader.h"

#include <vector>


class SceneRenderer
{
public:
    SceneRenderer();

public:

    void DrawMeshes(PerspectiveCamera& camera, // TODO: use camera abstract class
                    ShaderProgram& shader_program,
                    std::vector<MeshRenderObject>& mesh_render_packet);

};


