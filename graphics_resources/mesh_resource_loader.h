#pragma once

#include "resource_loader.h"

class MeshResourceLoader : public ResourceLoader
{
public:
    MeshResourceLoader();

public:

    std::unique_ptr<GraphicsResource> Create(const std::string& file_path) override;

private:

};


