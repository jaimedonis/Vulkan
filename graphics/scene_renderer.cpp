#include "scene_renderer.h"
#include "shader_program.h"

SceneRenderer::SceneRenderer()
{
}

void SceneRenderer::DrawMeshes(PerspectiveCamera& camera,
                               ShaderProgram& shader_program,
                               std::vector<MeshRenderObject>& mesh_render_packet)
{
    shader_program.BeginUsing();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable polygon offset to resolve depth-fighting isuses

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);

    int model_offset = 0;
    int view_offset = 1;
    int projection_offset = 2;
    int camera_position_offset = 3;

    int light_properties_offset = 8;

    shader_program.SetShaderUniform(camera_position_offset, camera.GetPosition());

    auto light_position = glm::vec3(-20, 60, 40);
    auto light_power = glm::vec3(1.0,1.0,1.0) * 30500.0f;
    auto light_color = glm::vec3(0.40,0.40,0.0);
    //_shader_program->SetShaderBufferData(_light_properties_buffer_object,
    //                                     light_properties_offset,
    //                                     1,
    //                                     &_light_properties,
    //                                     sizeof(LightProperties));
    shader_program.SetShaderUniform(light_properties_offset, light_position);
    shader_program.SetShaderUniform(light_properties_offset+1, light_power);
    shader_program.SetShaderUniform(light_properties_offset+2, light_color);

    shader_program.SetShaderUniform(view_offset, camera.GetView());
    shader_program.SetShaderUniform(projection_offset, camera.GetProjection());

    for(auto& render_object : mesh_render_packet ) {

        render_object._material_resource->Update(&shader_program);

        // TBD: use a new shader instance for each render object?
        //      if so, move shader in here.
        // update shader
        shader_program.SetShaderUniform(model_offset, render_object._world_transform);

        // render object
        render_object.OnRender();
    }
}
