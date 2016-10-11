#pragma once
#include "BaseObject.h"
#include "Collision.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <d3d9.h>
#include <d3dx9.h>
#include "Camera.h"

using namespace std;

class Sprite : 
	protected BaseObject, Collision, Camera
{
private:
	LPDIRECT3DTEXTURE9 Image;				// The container of all the sprites
	LPD3DXSPRITE SpriteHandler;
	RECT srect;
	D3DXVECTOR3 PosOut;
protected:
	float VxLast;
	float VyLast;

	int Index;								// Current sprite index
	int Width;								// Sprite width
	int Height;								// Sprite height
	int Count;								// Number of sprites
	int SpritePerRow;						// Number of sprites per row

public:
	void Next();
	void Reset();
	void RenderFrame(ViewPort *VPort);
	void CreateSprite(LPD3DXSPRITE spriteHandler, char *filePath, int width, int height, int count, int spritePerRow);


	float SweptAABB(BaseObject *Static);
	float AABB(BaseObject *Static);
	float CheckCollision(BaseObject *Static);
	virtual Point* UpdateCollision();

	Sprite();
	~Sprite();
};

