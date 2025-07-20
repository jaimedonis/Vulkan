#include "my_application.h"
#include "entity_components/base_entity.h"

// NOTE: possible name = interactive media engine  (IME)

MyApplication::MyApplication()
{
    BaseEntity root;
    root.CreateComponent<TransformComponent>();

    auto t = root.GetComponent<TransformComponent>();
    assert(t != nullptr);

    root.RemoveComponent<TransformComponent>();

    t = root.GetComponent<TransformComponent>();
    assert(t == nullptr);
}

void MyApplication::OnSetup()
{

}

void MyApplication::OnUpdate()
{

}

void MyApplication::OnTeardown()
{

}
