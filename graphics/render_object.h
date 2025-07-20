#pragma once

#include <glm/glm.hpp>

class RenderObject
{
protected:
    RenderObject();
    virtual ~RenderObject() = default;

public:

    // TODO: consider renaming these callbacks

    virtual void OnLoad() = 0;

    virtual void OnUnload() = 0;

    virtual void OnRender() = 0;

public: // TBD: protect
    // TODO: how to efficiently synchronize transform component (public interface, with parent-chlid hierarchy)
    // with this internal render object transform.
    glm::mat4 _world_transform;
};


