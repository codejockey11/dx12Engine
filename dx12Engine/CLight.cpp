#include "CLight.h"

/*
*/
CLight::CLight()
{
	memset(this, 0x00, sizeof(CLight));
}

/*
*/
CLight::CLight(float width, float height, float fov, float nearPlane, float farPlane,
	CVertex* diffuse, CVertex* position, CVertex* direction, float radius)
{
	memset(this, 0x00, sizeof(CLight));

	m_width = width;
	m_height = height;
	m_fov = fov;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;

	m_diffuse.x = diffuse->p.x;
	m_diffuse.y = diffuse->p.y;
	m_diffuse.z = diffuse->p.z;

	m_position.x = position->p.x;
	m_position.y = position->p.y;
	m_position.z = position->p.z;

	m_direction.x = direction->p.x;
	m_direction.y = direction->p.y;
	m_direction.z = direction->p.z;

	m_radius = radius;

	CLight::UpdateViewMatrix();
	CLight::GenerateProjectionMatrix();
}

/*
*/
CLight::~CLight()
{
}

/*
*/
void CLight::GenerateProjectionMatrix()
{
	m_xmproj = XMMatrixPerspectiveFovLH(m_fov * _PiDiv180, m_width / m_height, m_nearPlane, m_farPlane);

	XMStoreFloat4x4(&m_projectionMatrix, m_xmproj);
}

/*
*/
void CLight::UpdateRotation(float x, float y, float z)
{
	m_rotation.x += x;

	if (m_rotation.x < 0.0f)
	{
		m_rotation.x = 359.9f;
	}
	else if (m_rotation.x > 359.9f)
	{
		m_rotation.x = 0.0f;
	}

	m_rotation.y += y;

	if (m_rotation.y < 0.0f)
	{
		m_rotation.y = 359.9f;
	}
	else if (m_rotation.y > 359.9f)
	{
		m_rotation.y = 0.0f;
	}

	m_rotation.z += z;

	if (m_rotation.z < 0.0f)
	{
		m_rotation.z = 359.9f;
	}
	else if (m_rotation.z > 359.9f)
	{
		m_rotation.z = 0.0f;
	}
}

/*
*/
void CLight::UpdateViewMatrix()
{
	// yaw pitch roll in radians
	m_yaw = m_rotation.y * _PiDiv180;
	m_pitch = m_rotation.x * _PiDiv180;
	m_roll = m_rotation.z * _PiDiv180;

	m_xmMatrix = XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, m_roll);


	m_xmflook.x = 0.0f;
	m_xmflook.y = 0.0f;
	m_xmflook.z = 1.0f;

	m_xmvlook = XMLoadFloat3(&m_xmflook);

	m_xmvlook = XMVector3TransformCoord(m_xmvlook, m_xmMatrix);

	m_xmflook = XMFLOAT3(XMVectorGetX(m_xmvlook), XMVectorGetY(m_xmvlook), XMVectorGetZ(m_xmvlook));


	m_xmflook.x = m_xmflook.x + m_position.x;
	m_xmflook.y = m_xmflook.y + m_position.y;
	m_xmflook.z = m_xmflook.z + m_position.z;

	m_xmvlook = XMLoadFloat3(&m_xmflook);


	m_xmfup.x = 0.0f;
	m_xmfup.y = 1.0f;
	m_xmfup.z = 0.0f;

	m_xmvup = XMLoadFloat3(&m_xmfup);

	m_xmvup = XMVector3TransformCoord(m_xmvup, m_xmMatrix);

	m_xmfup = XMFLOAT3(XMVectorGetX(m_xmvup), XMVectorGetY(m_xmvup), XMVectorGetZ(m_xmvup));


	m_xmvposition = XMLoadFloat3(&m_position);


	m_xmview = XMMatrixLookAtLH(m_xmvposition, m_xmvlook, m_xmvup);

	XMStoreFloat4x4(&m_viewMatrix, m_xmview);
}