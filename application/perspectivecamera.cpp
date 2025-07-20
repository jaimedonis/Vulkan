#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera()
{

}

void PerspectiveCamera::Setup(float ar, float fov, float z_near, float z_far)
{
    _ar = ar;
    _fov = fov;
    _z_near = z_near;
    _z_far = z_far;

    _projection = glm::perspective(_fov, _ar, _z_far, _z_near);
}

void PerspectiveCamera::SetPosition(const glm::vec3& position)
{
    _position = position;

    UpdateTransforms();
}

void PerspectiveCamera::LookAt(const glm::vec3& target, const glm::vec3& up)
{
    _view_target = target;
    _view_up = up;

    UpdateTransforms();
}

glm::mat4 PerspectiveCamera::GetView() const
{
    return _view;
}

glm::mat4 PerspectiveCamera::GetProjection() const
{
    return _projection;
}

void PerspectiveCamera::UpdateTransforms()
{
    _view = glm::lookAt(_position, _view_target, _view_up);
}
