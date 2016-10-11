#include "Camera.h"


void Camera::SetViewPort(int vpx, int vpy, int vpHeight, int vpWidth)
{
	VPort->Vpx = vpx;
	VPort->Vpy = vpy;
	VPort->Height = vpHeight;
	VPort->Width = vpWidth;
}

void Camera::SetViewPort(ViewPort *viewPort)
{
	VPort = viewPort;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return PosOut;
}

D3DXVECTOR3 Camera::WorldTransform(RECT Rect, D3DXVECTOR3 PosIn)
{
	D3DXMatrixIdentity(&mt);
	mt._22 = -1.0f;
	mt._41 = -VPort->Vpx;
	mt._42 = VPort->Vpy;
	D3DXVECTOR4 vp_pos;
	D3DXVec3Transform(&vp_pos, &PosIn, &mt);
	return D3DXVECTOR3(vp_pos.x, vp_pos.y, 0);
}

Camera::Camera()
{
	VPort = NULL;
}


Camera::~Camera()
{
	delete VPort;
}
