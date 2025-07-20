#pragma once

#include "graphics_resources/graphics_resource_manager.h"
#include "graphics/render_graph.h"

#include <memory>


typedef struct EngineConfig {
    int _window_width = 1024;
    int _window_height = 768;
} EngineConfig;

class Engine
{
private:

    Engine();

public:

    static Engine* GetInstance();

    inline EngineConfig* GetConfig() { return &_config; }

    inline GraphicsResourceManager* GetResourceMgr() { return &_resource_manager; }

    inline RenderGraph* GetRenderGraph() { return &_render_graph; }

    void OnSetup(const EngineConfig& config);

    void OnUpdate();

    void OnTeardown();

private:

    static std::unique_ptr<Engine> _instance;

    EngineConfig _config;
    GraphicsResourceManager _resource_manager;
    RenderGraph _render_graph;
};
