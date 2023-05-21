#include <graphics/camera.h>

Camera::Camera()
{
}

void Camera::Recalculate()
{
    if (_pitch > 89.f)
    {
        _pitch = 89.f;
    }
    if (_pitch < -89.f)
    {
        _pitch = -89.f;
    }

    glm::vec3 _viewDirection = glm::normalize(glm::vec3(
        cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
        sin(glm::radians(_pitch)),
        sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))));
}

void Camera::RecalculateProjectionMatrix()
{
    _projectionMatrix = glm::perspective(glm::radians(_FOV), (float)(_height * _aspectRatio) / (float)_height, _near, _far);
}

void Camera::SetAspectRatio(float ar)
{
	_aspectRatio = ar;
}

void Camera::SetHeight(float h)
{
	_height = h;
}

void Camera::SetNear(float n)
{
	_near = n;
}

void Camera::SetFar(float f)
{
	_far = f;
}

void Camera::SetFOV(float fov)
{
    _FOV = fov;
}

void Camera::SetPerspective(float height, float fov, float n, float f)
{
	_height = height;
	_FOV = fov;
	_near = n;
	_far = f;
}
