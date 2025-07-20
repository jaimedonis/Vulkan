#pragma once

#include "managed_object.h"

#include <string>
#include <map>
#include <memory>


class ObjectManager
{
private:

    ObjectManager() = delete;

public:

    static void Add(const std::string& object_id, const std::shared_ptr<ManagedObject>& object_instance);

    static bool Remove(const std::string& object_id);

    static void RemoveAll();

    template<class T>
    static T* Get(const std::string& object_id)
    {
        auto iter = ObjectManager::_object_store.find(object_id);

        if( iter != ObjectManager::_object_store.end()) {
            return dynamic_cast<T*>(iter->second.get());
        }

        return nullptr;
    }

private:

    static std::map<std::string, std::shared_ptr<ManagedObject>> _object_store;

};
