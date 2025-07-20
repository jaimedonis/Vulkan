#include "graphics_resource_manager.h"

#include <iostream>

GraphicsResourceManager::GraphicsResourceManager()
{
}

void GraphicsResourceManager::RegisterResourceLoader(const std::string& resource_loader_name,
                                                     std::unique_ptr<ResourceLoader> resource_loader,
                                                     const std::vector<std::string>& file_extensions)
{
    _resource_loaders[resource_loader_name] = std::move(resource_loader);
    for(const auto& ext : file_extensions) {
        _file_extensions_xref[ext] = resource_loader_name;
    }
}
/*
template<class T>
T* GraphicsResourceManager::GetResource(const std::string& file_path)
{
    // == lookup this item within the resource cache first ==

    auto cache_iter = _cached_resources.find(file_path);
    if( cache_iter != _cached_resources.end() ) {
        return dynamic_cast<T>(cache_iter->second.get());
    }

    // == create a new resource instance ==

    auto idx = file_path.find_last_of(".");
    std::string file_ext;
    if( idx >= 0 ) {
        file_ext = file_path.substr(idx);
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

    std::unique_ptr<void*> new_resource_instance = loader->Create(file_path);
    void* new_resource_instance_ptr = new_resource_instance.get();

    _cached_resources[file_path] = std::move(new_resource_instance);

    return dynamic_cast<T>(new_resource_instance_ptr);
}
*/
void GraphicsResourceManager::RemoveResources(const std::string& folder_path)
{
    // == remove all, if folder path is empty ==

    if( folder_path.empty() ) {
        _cached_resources.clear();
        return;
    }

    // == remove specific range of resources, based on the given folder path ==

    std::vector<std::string> remove_list;
    for( const auto& resource : _cached_resources ) {
        if( resource.first.find(folder_path) ) {
            remove_list.emplace_back(resource.first);
        }
    }

    for( const auto& file_path : remove_list ) {
        _cached_resources.erase(file_path);
    }
}
