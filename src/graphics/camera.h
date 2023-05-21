#pragma once
#include <core/common.h>

class Camera
{
private:
	float _FOV = 90.0f;
	float _near = 0.1f;
	float _far = 10.0f;
	float _height;
	float _aspectRatio;
	float _speed;

    float _rotY = 0;
    float _rotX = 0;
	float _yaw;
	float _pitch;

	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;

	glm::vec3 _pos = glm::vec3(0, 0, 1);

public:
    Camera();
    
    void Recalculate();
    void RecalculateProjectionMatrix();
    
    void SetAspectRatio(float ar);
    void SetHeight(float h);
    void SetFOV(float fov);
    void SetNear(float n);
    void SetFar(float f);

    void SetPerspective(float height, float fov, float n, float f);

    float& GetYaw() { return _yaw; }
    float& GetPitch() { return _pitch; }

    void SetPitchAndYaw(float pitch, float yaw);
};