#pragma once

#include "../engine.h"

class Application
{
public:
    Application();
    virtual ~Application() = default;

public:

    inline bool IsRunning() { return _is_running; }

    bool Initialize(const EngineConfig& config);

    void Update();

    bool Deinitialize();

    virtual void OnSetup() = 0;

    virtual void OnUpdate() = 0;

    virtual void OnTeardown() = 0;

private:

    void* _gl_context;

    bool _is_running;
};

