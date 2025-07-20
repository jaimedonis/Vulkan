#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>


//! This is a simple perspective camera class, no need to derive from a base camera
//! since this is the only camera in use at this moment.
class PerspectiveCamera
{
public:
    PerspectiveCamera();

    void Setup(float ar, float fov, float z_near, float z_far);

    inline glm::vec3 GetPosition() { return _position; }
    void SetPosition(const glm::vec3& position);

    void LookAt(const glm::vec3& target, const glm::vec3& up);

    glm::mat4 GetView() const;
    glm::mat4 GetProjection() const;

private:

    void UpdateTransforms();

private:

    glm::vec3 _position;
    glm::vec3 _view_target;
    glm::vec3 _view_up;

    float _ar;
    float _fov;
    float _z_near;
    float _z_far;

    glm::mat4 _view;
    glm::mat4 _projection;
};


