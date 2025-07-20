#include "render_graph.h"

#include "mesh_render_object.h"

#include "../graphics/shader.h"
#include "../graphics/object_manager.h"
#include "../engine.h"
#include "../diagnostics/logger.h"


struct SurfaceMaterial0 {
    glm::vec3 diffuse_color;
    glm::vec3 specular_color;
    float specular_power;
    float surface_albedo;
};

struct LightProperties {
    glm::vec3 light_position;
    glm::vec3 light_power;
    glm::vec3 light_color;
};

static GLuint _surface_material_buffer_object = 0;
static GLuint _light_properties_buffer_object = 0;
static SurfaceMaterial0 _surface_material;
static LightProperties _light_properties;


// TODO: Make this the scene manager, which uses the scene renderer to draw its objects

RenderGraph::RenderGraph()
{
}

void RenderGraph::OnLoad()
{
    // == setup global scene-like objects ==
    // camera, lights, etc...
    _camera = std::make_shared<PerspectiveCamera>();
    float ar = float(Engine::GetInstance()->GetConfig()->_window_width) /
               float(Engine::GetInstance()->GetConfig()->_window_height);
    _camera->Setup(ar, glm::radians(60.0), 1.0, 1000.0);
    _camera->SetPosition(glm::vec3(0, 60, 125));
    _camera->LookAt(glm::vec3(0, 60, 0), glm::vec3(0, 1, 0));

    // ==preload scene assets here (optional)==
    auto my_mesh = Engine::GetInstance()->GetResourceMgr()->GetResource<Mesh>("assets/meshes/Outlander_Model.fbx");
    assert(my_mesh != nullptr);

    // == create render techniques & passes ==

    // create a shader program / render passes with techniques ....
    _shader_program = std::make_unique<ShaderProgram>();
    ObjectManager::Add("my_vertex_shader",
                       std::make_shared<Shader>(ShaderType::VERTEX, "assets/shaders/bsdf_single_ray.vert"));
    ObjectManager::Add("my_fragment_shader",
                       std::make_shared<Shader>(ShaderType::FRAGMENT, "assets/shaders/bsdf_single_ray.frag"));
    _shader_program->AddShader("my_vertex_shader");
    _shader_program->AddShader("my_fragment_shader");
    _shader_program->Link();

    // tbd: create a scene graph, which contain the render objects?  client app uses/queries the scene
    //      application has determines the scene graph(s) definitions
    //      this render graph class accesses the scene objects and preloads the meshes, then
    //      creates mesh render objects....

    // create render graph (use pre-loaded assets referenced by their internal resource id)
    // TODO...
    auto render_object1 = std::make_unique<MeshRenderObject>();
    render_object1->_world_transform = glm::mat4(1.0) * glm::translate(glm::vec3(0, 0, 5));
    render_object1->_mesh_resource = my_mesh;
    _opaque_objects.emplace_back(std::move(render_object1));

    // setup/load all render objects
    for(auto& render_object : _opaque_objects) {
        render_object->OnLoad();
    }

    GL_INVOKE(glGenBuffers(1, &_surface_material_buffer_object));
    GL_INVOKE(glGenBuffers(1, &_light_properties_buffer_object));
}

void RenderGraph::OnUnload()
{
    for(auto& render_object : _opaque_objects) {
        render_object->OnUnload();
    }

    // destroy the shader
    _shader_program->Destroy();

    auto shader = ObjectManager::Get<Shader>("my_vertex_shader");
    shader->Destroy();
    ObjectManager::Remove("my_vertex_shader");

    shader = ObjectManager::Get<Shader>("my_fragment_shader");
    shader->Destroy();
    ObjectManager::Remove("my_fragment_shader");
}

void RenderGraph::Render()
{
    // TODO: - pass the scene's opaque mesh render objects to a mesh render target class
    //         the mesh render target iterates over its own render objects
    //       - pass camera and lights
    //       - render targets have internal access render object's gl resource objects,
    //         which are created during the loading process (RenderGraph::OnLoad()).
    //       - the client applicatino must have direct access to the render object's transform, etc...

    // use shader
    _shader_program->BeginUsing();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Enable polygon offset to resolve depth-fighting isuses

    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);

    int model_offset = 0;
    int view_offset = 1;
    int projection_offset = 2;
    int camera_position_offset = 3;
    int material_offset = 4;
    int light_properties_offset = 8;

    _shader_program->SetShaderUniform(camera_position_offset, _camera->GetPosition());

    _surface_material.diffuse_color = glm::vec3(0.0,1.0,0.0);
    _surface_material.specular_color = glm::vec3(0.05,0.05,0.05);
    float s_sigma = 0.45; // smaller is more brillante
    _surface_material.specular_power = pow(8192.0, 1.0-s_sigma);
    _surface_material.surface_albedo = 2.0;

    //_shader_program->SetShaderBufferData(_surface_material_buffer_object,
    //                                     material_offset,
    //                                     0,
    //                                     &_surface_material,
    //                                     sizeof(SurfaceMaterial));
    _shader_program->SetShaderUniform(material_offset, _surface_material.diffuse_color);
    _shader_program->SetShaderUniform(material_offset+1, _surface_material.specular_color);
    _shader_program->SetShaderUniform(material_offset+2, _surface_material.specular_power);
    _shader_program->SetShaderUniform(material_offset+3, _surface_material.surface_albedo);

    _light_properties.light_position = glm::vec3(-20, 60, 40);
    _light_properties.light_power = glm::vec3(1.0,1.0,1.0) * 30500.0f;
    _light_properties.light_color = glm::vec3(0.40,0.40,0.0);
    //_shader_program->SetShaderBufferData(_light_properties_buffer_object,
    //                                     light_properties_offset,
    //                                     1,
    //                                     &_light_properties,
    //                                     sizeof(LightProperties));
    _shader_program->SetShaderUniform(light_properties_offset, _light_properties.light_position);
    _shader_program->SetShaderUniform(light_properties_offset+1, _light_properties.light_power);
    _shader_program->SetShaderUniform(light_properties_offset+2, _light_properties.light_color);

    _shader_program->SetShaderUniform(view_offset, _camera->GetView());
    _shader_program->SetShaderUniform(projection_offset, _camera->GetProjection());

    for(auto& render_object : _opaque_objects ) {
        // TBD: use a new shader instance for each render object?
        //      if so, move shader in here.
        // update shader
        _shader_program->SetShaderUniform(model_offset, render_object->_world_transform);

        // render object
        render_object->OnRender();
    }

    // stop using shader
    //_shader_program->StopUsing();
}
