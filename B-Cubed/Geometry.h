#pragma once
#include <vector>
#include <DirectXMath.h>
#include "IndexedVertexList.h"

class Geometry
{
public:
	template <class V>
	static IndexedVertexList<V> MakeBoxIndependentFaces(float length, float width, float height)
	{
		const float x = width / 2.0f;
		const float y = height / 2.0f;
		const float z = length / 2.0f;

		IndexedVertexList<V> ivl;

		std::vector<V> vertices(24);

		// front
		vertices[0].pos = DirectX::XMFLOAT3(-x, -y, -z);
		vertices[1].pos = DirectX::XMFLOAT3(-x,  y, -z);
		vertices[2].pos = DirectX::XMFLOAT3( x,  y, -z);
		vertices[3].pos = DirectX::XMFLOAT3( x, -y, -z);
		// right
		vertices[4].pos = DirectX::XMFLOAT3(x,-y,-z);
		vertices[5].pos = DirectX::XMFLOAT3(x,y,-z);
		vertices[6].pos = DirectX::XMFLOAT3(x,y,z);
		vertices[7].pos = DirectX::XMFLOAT3(x,-y,z);
		// left
		vertices[8].pos = DirectX::XMFLOAT3(-x,-y,z);
		vertices[9].pos = DirectX::XMFLOAT3(-x,y,z);
		vertices[10].pos = DirectX::XMFLOAT3(-x,y,-z);
		vertices[11].pos = DirectX::XMFLOAT3(-x,-y,-z);
		// bottom
		vertices[12].pos = DirectX::XMFLOAT3(-x,-y,z);
		vertices[13].pos = DirectX::XMFLOAT3(-x,-y,-z);
		vertices[14].pos = DirectX::XMFLOAT3(x,-y,-z);
		vertices[15].pos = DirectX::XMFLOAT3(x,-y,z);
		// top
		vertices[16].pos = DirectX::XMFLOAT3(-x,y,-z);
		vertices[17].pos = DirectX::XMFLOAT3(-x,y,z);
		vertices[18].pos = DirectX::XMFLOAT3(x,y,z);
		vertices[19].pos = DirectX::XMFLOAT3(x,y,-z);
		// back
		vertices[20].pos = DirectX::XMFLOAT3(x,-y,z);
		vertices[21].pos = DirectX::XMFLOAT3(x,y,z);
		vertices[22].pos = DirectX::XMFLOAT3(-x,y,z);
		vertices[23].pos = DirectX::XMFLOAT3(-x,-y,z);

		for (int i = 0; i < 24; i+=4)
		{
			vertices[i+0].tex = DirectX::XMFLOAT2(0.0f,0.0f);
			vertices[i+1].tex = DirectX::XMFLOAT2(0.0f,1.0f);
			vertices[i+2].tex = DirectX::XMFLOAT2(1.0f,1.0f);
			vertices[i+3].tex = DirectX::XMFLOAT2(1.0f, 0.0f);
		}

		std::vector<unsigned short> indices =
		{
			// front
			0,1,2,
			0,2,3,
			// right
			4,5,6,
			4,6,7,
			// left
			8,9,10,
			8,10,11,
			// bottom
			12,13,14,
			12,14,15,
			// top
			16,17,18,
			16,18,19,
			// back
			20,21,22,
			20,22,23
		};
		return { std::move(vertices),std::move(indices) };
	}
	
	
private:
};
