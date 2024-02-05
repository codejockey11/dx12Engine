#pragma once

#include "standard.h"

#include "CVertex.h"

class CLight
{
public:

	XMFLOAT3 m_direction;
	XMFLOAT3 m_look;
	XMFLOAT3 m_position;
	XMFLOAT3 m_right;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_up;
	
	XMFLOAT4 m_diffuse;
	
	XMFLOAT4X4 m_projectionMatrix;
	XMFLOAT4X4 m_viewMatrix;
	
	float m_farPlane;
	float m_fov;
	float m_height;
	float m_nearPlane;
	float m_pitch;
	float m_radius;
	float m_roll;
	float m_width;
	float m_yaw;
	
	CLight();
	CLight(float width, float height, float fov, float nearPlane, float farPlane, CVertex* diffuse, CVertex* position, CVertex* direction, float radius);
	~CLight();

	void GenerateProjectionMatrix();
	void UpdateRotation(float x, float y, float z);
	void UpdateViewMatrix();

private:

	XMFLOAT3 m_xmflook;
	XMFLOAT3 m_xmfup;
	
	XMMATRIX m_xmMatrix;
	XMMATRIX m_xmproj;
	XMMATRIX m_xmview;
	
	XMVECTOR m_xmvlook;
	XMVECTOR m_xmvposition;
	XMVECTOR m_xmvup;
};