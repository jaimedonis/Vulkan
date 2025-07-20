#include "object_manager.h"

#include <cassert>


std::map<std::string, std::shared_ptr<ManagedObject>> ObjectManager::_object_store;

void ObjectManager::Add(const std::string& object_id, const std::shared_ptr<ManagedObject>& object_instance)
{
    auto iter = _object_store.find(object_id);
    assert(iter == _object_store.end());

    _object_store[object_id] = object_instance;
}

bool ObjectManager::Remove(const std::string& object_id)
{
    int counter = _object_store.erase(object_id);

    return counter > 0;
}

void ObjectManager::RemoveAll()
{
    _object_store.clear();
}
