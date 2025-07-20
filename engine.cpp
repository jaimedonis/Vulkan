#include "engine.h"
#include "graphics_resources/mesh_resource_loader.h"

std::unique_ptr<Engine> Engine::_instance = nullptr;

Engine::Engine()
{

}

Engine* Engine::GetInstance()
{
    if(_instance == nullptr) {
        _instance = std::unique_ptr<Engine>(new Engine());
    }

    return _instance.get();
}

void Engine::OnSetup(const EngineConfig& config)
{
    _config = config;

    _resource_manager.RegisterResourceLoader("MeshResourceLoader",
                                             std::make_unique<MeshResourceLoader>(),
                                             {"obj", "stl", "fbx", "ply"});

    _render_graph.OnLoad();
}

void Engine::OnUpdate()
{
    _render_graph.Render();
}

void Engine::OnTeardown()
{
    _render_graph.OnUnload();
}
