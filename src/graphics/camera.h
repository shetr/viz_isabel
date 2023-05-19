#pragma once
#include <core/common.h>

class Camera
{
private:
	float _FOV;
	float _near;
	float _far;
	float _height;
	float _aspectRatio;
	float _speed;

	float _yaw;
	float _pitch;

	glm::mat4 _projectionMatrix;
	glm::mat4 _viewMatrix;

	glm::vec3 _position;					
	glm::vec3 _upVector;
	glm::vec3 _viewDirection;

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