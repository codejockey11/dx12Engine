#pragma once

#include "standard.h"

#include "CVertex.h"

class CCamera
{
public:

	XMFLOAT3 m_rotation;
	XMFLOAT3 m_position;

	XMFLOAT3 m_right;
	XMFLOAT3 m_up;
	XMFLOAT3 m_look;

	float m_yaw;
	float m_pitch;
	float m_roll;

	XMMATRIX m_wvpMat;

	XMFLOAT4X4 m_xmfworld;
	XMFLOAT4X4 m_xmfview;
	XMFLOAT4X4 m_xmfproj;

	XMMATRIX m_xmworld;
	XMMATRIX m_xmview;
	XMMATRIX m_xmproj;

	XMMATRIX m_twvpMat;

	float m_width;
	float m_height;
	float m_fov;
	float m_nearPlane;
	float m_farPlane;


	CCamera();

	CCamera(float width, float height, float fov, float nearPlane, float farPlane);

	~CCamera();


	void UpdateView();

	void UpdateRotation(float x, float y, float z);

	void Copy(CCamera* camera);

private:

	XMFLOAT3 m_xmfup;
	XMFLOAT3 m_xmflook;

	XMMATRIX m_xmMatrix;

	XMVECTOR m_xmvlook;
	XMVECTOR m_xmvup;
	XMVECTOR m_xmvposition;
};