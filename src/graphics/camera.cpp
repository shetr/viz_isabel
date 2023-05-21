#include <graphics/camera.h>

Camera::Camera()
{
}

void Camera::Recalculate(float aspectRatio)
{
    if (_rotX > 89.f)
    {
        _rotX = 89.f;
    }
    if (_rotX < -89.f)
    {
        _rotX = -89.f;
    }
    if (_rotY > 180.f)
    {
        _rotY -= 360.f;
    }
    if (_rotY < -180.f)
    {
        _rotY += 360.f;
    }

    _P = glm::perspective(glm::radians(_FOV), aspectRatio, _near, _far);
    _V = glm::translate(-_pos) * glm::rotate(-glm::radians(_rotX), glm::vec3(-1, 0, 0)) * glm::rotate(-glm::radians(_rotY), glm::vec3(0, 1, 0));
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

void Camera::SetPerspective(float fov, float n, float f)
{
	_FOV = fov;
	_near = n;
	_far = f;
}
