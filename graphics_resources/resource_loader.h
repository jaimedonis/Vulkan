#pragma once

#include "graphics_resource.h"

#include <string>
#include <memory>


class ResourceLoader
{
protected:
    ResourceLoader(const std::string& resource_loader_name);

public:

    inline std::string GetResourceLoaderName() { return _resource_loader_name; }

    virtual std::unique_ptr<GraphicsResource> Create(const std::string& file_path) = 0;

private:
    std::string _resource_loader_name;

};

