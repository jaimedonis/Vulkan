#pragma once

#include <string>

class GraphicsResource
{
public:
    GraphicsResource() = default;
    virtual ~GraphicsResource() = default;

public:
    std::string _resource_id;
};
