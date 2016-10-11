#include "Sprite.h"

void Sprite::CreateSprite(LPD3DXSPRITE spriteHandler, char *filePath, int width, int height, int count, int spritePerRow)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	Image = NULL;
	SpriteHandler = spriteHandler;

	Width = width;
	Height = height;
	Count = count;
	SpritePerRow = spritePerRow;
	Index = 0;

	result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv;
	SpriteHandler->GetDevice(&d3ddv);

	result = D3DXCreateTextureFromFileEx(
		d3ddv,
		filePath,
		info.Width,
		info.Height,
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0, 63, 63),
		&info,
		NULL,
		&Image);

	if (result != D3D_OK)
	{
		return;
	}
}

void Sprite::Next()
{
	Index = (Index + SpritePerRow - 1) % SpritePerRow;
}

void Sprite::Reset()
{
	Index = 0;
}

void Sprite::RenderFrame(ViewPort *VPort)
{
	srect.left = (Index % SpritePerRow)*(Width);
	srect.top = (Index / SpritePerRow)*(Height);
	srect.right = srect.left + Width;
	srect.bottom = srect.top + Height;

	D3DXVECTOR3 position((float)this->CurrentPos->X, (float)this->CurrentPos->Y, 0);

	SetViewPort(VPort);
	PosOut = WorldTransform(srect, position);

	D3DXVECTOR3 center((float)Width / 2, (float)Height, 0);

	SpriteHandler->Draw(
		Image,
		&srect,
		&center,
		&PosOut,
		D3DCOLOR_XRGB(255, 255, 255)
	);
}


float Sprite::SweptAABB(BaseObject *Static)
{
	//he quy chieu ox huong sang phai, oy hung len tren
	//position la diem o chinh giua duoi cung cua img
	// Find the distance giữa objects near and far sides on the x and y for both Show
	if (this->Vx > 0)
	{
		xInvEntry = (Static->CurrentPos->X - Static->Width / 2) - (this->CurrentPos->X + this->Width / 2);
		xInvExit = (Static->CurrentPos->X + Static->Width / 2) - (this->CurrentPos->X - this->Width / 2);
	}
	else
	{
		xInvEntry = (Static->CurrentPos->X + Static->Width / 2) - (this->CurrentPos->X - this->Width / 2);
		xInvExit = (Static->CurrentPos->X - Static->Width / 2) - (this->CurrentPos->X + this->Width / 2);
	}

	if (this->Vy > 0)
	{
		yInvEntry = (Static->CurrentPos->Y) - (this->CurrentPos->Y + this->Height);
		yInvExit = (Static->CurrentPos->Y + Static->Height) - (this->CurrentPos->Y);
	}
	else
	{
		yInvEntry = (Static->CurrentPos->Y + Static->Height) - (this->CurrentPos->Y);
		yInvExit = (Static->CurrentPos->Y) - (this->CurrentPos->Y + this->Height);
	}


	// Find time of collision and time of Leaving for each axis (if statement is to Prevent divide by zero)
	if (this->Vx == 0.0f)
	{
		xEntry = numeric_limits <float> ::infinity();
		xExit = numeric_limits <float> ::infinity();
	}
	else
	{
		xEntry = xInvEntry / this->Vx;
		xExit = xInvExit / this->Vx;
	}

	if (this->Vy == 0.0f)
	{
		yEntry = numeric_limits <float> ::infinity();
		yExit = numeric_limits <float> ::infinity();
	}
	else
	{
		yEntry = yInvEntry / this->Vy;
		yExit = yInvExit / this->Vy;
	}

	// Find the EARLIEST / Latest times of collision
	entryTime = max(xEntry, yEntry);
	exitTime = min(xExit, yExit);

	// If there was no collision
	if (entryTime > exitTime ||
		xEntry < 0.0f && yEntry < 0.0f ||
		xEntry> 1.0f || yEntry> 1.0f)
	{
		NormalX = 0.0f;
		NormalY = 0.0f;
		return 1.0f;
	}
	else // if there was a collision
	{
		// Calculate surface normal of collided
		if (xEntry> yEntry)
		{
			if (xInvEntry < 0.0f)
			{
				NormalX = 1.0f;
				NormalY = 0.0f;
			}
			else
			{
				NormalX = -1.0f;
				NormalY = 0.0f;
			}
		}
		else
		{
			if (yInvEntry <0.0f)
			{
				NormalX = 0.0f;
				NormalY = 1.0f;
			}
			else
			{
				NormalX = 0.0f;
				NormalY = -1.0f;
			}
		}

		// Return the time of collision
		return entryTime;
	}
}

float Sprite::AABB(BaseObject *Static)
{
	float dynamicLeft = this->CurrentPos->X - this->Width / 2;
	float dynamicRight = this->CurrentPos->X + this->Width / 2;
	float dynamicBottom = this->CurrentPos->Y;
	float dynamicTop = this->CurrentPos->Y + this->Height;

	float statisLeft = Static->CurrentPos->X - Static->Width / 2;
	float statisRight = Static->CurrentPos->X + Static->Width / 2;
	float statisBottom = Static->CurrentPos->Y;
	float statisTop = Static->CurrentPos->Y + Static->Height;

	//ko va cham
	if (dynamicRight < statisLeft)
	{
		return Collision::None;
	}
	if (dynamicLeft > statisRight)
	{
		return Collision::None;
	}
	if (dynamicBottom > statisTop)
	{
		return Collision::None;
	}
	if (dynamicTop < statisBottom)
	{
		return Collision::None;
	}
}

float Sprite::CheckCollision(BaseObject *Static)
{
	if(AABB(Static))
	{
		return 1;
	}
	else
	{
		return SweptAABB(Static);
	}
}

Point* Sprite::UpdateCollision()
{
	Point p;
	return &p;
}

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	Image->Release();
}
