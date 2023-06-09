#pragma once
#include <core/common.h>

class Camera
{
private:
	float _FOV = 90.0f;
	float _near = 0.1f;
	float _far = 10.0f;
	float _movementSpeed = 1.0f;
	float _rotationSpeed = 100.0f;
	float _zoomSpeed = 5.0f;

    float _rotY = -50;
    float _rotX = 20;

	glm::mat4 _P;
	glm::mat4 _V;

	glm::vec3 _centerPos = glm::vec3(0, 0, 0);
	glm::vec3 _offset = glm::vec3(0, 0, 1);

public:
    Camera();
    
    void Recalculate(float aspectRatio);
	const glm::mat4& GetRoation() const;
    
    void SetAspectRatio(float ar);
    void SetHeight(float h);
    void SetFOV(float fov);
    void SetNear(float n);
    void SetFar(float f);

    void SetPerspective(float fov, float n, float f);

    float& GetRotY() { return _rotY; }
    float& GetRotX() { return _rotX; }
    float& GetMovementSpeed() { return _movementSpeed; }
    float& GetRotationSpeed() { return _rotationSpeed; }
    float& GetZoomSpeed() { return _zoomSpeed; }
    float& GetFOV() { return _FOV; }

    glm::vec3& GetCenterPos() { return _centerPos; }

	const glm::mat4& GetV() const { return _V; }
	const glm::mat4& GetP() const { return _P; }

    void SetPitchAndYaw(float pitch, float yaw);
};