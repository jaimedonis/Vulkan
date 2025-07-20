#pragma once

#include "resource_loader.h"
#include "graphics_resource.h"

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <iostream>


class GraphicsResourceManager
{
public:
    GraphicsResourceManager();

public:

    void RegisterResourceLoader(const std::string& resource_loader_name,
                                std::unique_ptr<ResourceLoader> resource_loader,
                                const std::vector<std::string>& file_extensions);

    template<class T>
    T* GetResource(const std::string& file_path)
    {
        // == lookup this item within the resource cache first ==

        auto cache_iter = _cached_resources.find(file_path);
        if( cache_iter != _cached_resources.end() ) {
            return (T*)cache_iter->second.get();
        }

        // == create a new resource instance ==

        auto idx = file_path.find_last_of(".");
        std::string file_ext;
        if( idx >= 0 ) {
            file_ext = file_path.substr(idx+1);
        }

        if(file_ext.empty()) {
            std::cout << "Unknown file extention for resource path: " << file_path << std::endl;
            return nullptr;
        }

        const auto& loader_name_iter = _file_extensions_xref.find(file_ext);
        std::string loader_name;
        if( loader_name_iter != _file_extensions_xref.end() ) {
            loader_name = loader_name_iter->second;
        }

        if( loader_name.empty() ) {
            std::cout << "Unable to associate resource loader for resource file extension: " << file_path << std::endl;
            return nullptr;
        }

        const auto& loader_iter = _resource_loaders.find(loader_name);
        ResourceLoader* loader = nullptr;
        if( loader_iter != _resource_loaders.end() ) {
            loader = loader_iter->second.get();
        }

        if( loader == nullptr ) {
            std::cout << "Unable to locate resource loader for resource file path: " << file_path << std::endl;
            return nullptr;
        }

        // == cache this new resource instance ==

        auto new_resource_instance = loader->Create(file_path);
        void* new_resource_instance_ptr = new_resource_instance.get();

        if(new_resource_instance_ptr != nullptr) {
            _cached_resources[file_path] = std::move(new_resource_instance);
        } else {
            std::cerr << "Unable to create new asset instance from resource file: " << file_path << std::endl;
        }

        return (T*)new_resource_instance_ptr;
    }

    //! When folder path is empty: all resources are removed from the cache
    //! Otherwise, only those matching the full folder path are removed
    void RemoveResources(const std::string& folder_path);

private:
    std::map<std::string, std::unique_ptr<ResourceLoader>> _resource_loaders;
    std::map<std::string, std::string> _file_extensions_xref;
    std::map<std::string, std::unique_ptr<GraphicsResource>> _cached_resources;
};
