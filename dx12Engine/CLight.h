#pragma once

#include "standard.h"

#include "CVertex.h"

class CLight
{
public:

	float m_width;
	float m_height;
	float m_fov;
	float m_nearPlane;
	float m_farPlane;

	XMFLOAT4 m_diffuse;

	XMFLOAT4X4 m_viewMatrix;
	XMFLOAT4X4 m_projectionMatrix;
		
	float m_yaw;
	float m_pitch;
	float m_roll;

	float m_radius;
	
	XMFLOAT3 m_position;
	XMFLOAT3 m_direction;
	XMFLOAT3 m_rotation;

	XMFLOAT3 m_right;
	XMFLOAT3 m_up;
	XMFLOAT3 m_look;

	
	CLight();

	CLight(float width, float height, float fov, float nearPlane, float farPlane, CVertex* diffuse, CVertex* position, CVertex* direction, float radius);
	
	~CLight();

	
	void UpdateRotation(float x, float y, float z);
	void UpdateViewMatrix();
	void GenerateProjectionMatrix();

private:

	XMFLOAT3 m_xmfup;
	XMFLOAT3 m_xmflook;

	XMMATRIX m_xmMatrix;

	XMVECTOR m_xmvlook;
	XMVECTOR m_xmvup;
	XMVECTOR m_xmvposition;

	XMMATRIX m_xmview;
	XMMATRIX m_xmproj;
};