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

		// front
		vertices[0].norm = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[1].norm = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[2].norm = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		vertices[3].norm = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
		// right
		vertices[4].norm = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[5].norm = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[6].norm = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		vertices[7].norm = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		// left		
		vertices[8].norm = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertices[9].norm = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertices[10].norm = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
		vertices[11].norm = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);
		// bottom	 
		vertices[12].norm = DirectX::XMFLOAT3(0.0, -1.0f, 0.0f);
		vertices[13].norm = DirectX::XMFLOAT3(0.0, -1.0f, 0.0f);
		vertices[14].norm = DirectX::XMFLOAT3(0.0, -1.0f, 0.0f);
		vertices[15].norm = DirectX::XMFLOAT3(0.0, -1.0f, 0.0f);
		// top		 
		vertices[16].norm = DirectX::XMFLOAT3(0.0, 1.0f, 0.0f);
		vertices[17].norm = DirectX::XMFLOAT3(0.0, 1.0f, 0.0f);
		vertices[18].norm = DirectX::XMFLOAT3(0.0, 1.0f, 0.0f);
		vertices[19].norm = DirectX::XMFLOAT3(0.0, 1.0f, 0.0f);
		// back		 
		vertices[20].norm = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[21].norm = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[22].norm = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		vertices[23].norm = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);

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

	template <class V>
	static IndexedVertexList<V> MakeSkyBox(float side)
	{
		const float hs = side / 2.0f;
		static constexpr float x = 0.25f;
		static constexpr float y = 1.0f / 3.0f;

		std::vector<V> vertices(14);

		vertices[0].pos = DirectX::XMFLOAT3(hs, hs, -hs);
		vertices[0].tex = DirectX::XMFLOAT2(0.0f*x, 2.0f*y);

		vertices[1].pos = DirectX::XMFLOAT3(hs, hs, hs);
		vertices[1].tex = DirectX::XMFLOAT2(1.0f*x, 2.0f*y);

		vertices[2].pos = DirectX::XMFLOAT3(-hs, hs, hs);
		vertices[2].tex = DirectX::XMFLOAT2(2.0f*x, 2.0f*y);

		vertices[3].pos = DirectX::XMFLOAT3(-hs, hs, -hs);
		vertices[3].tex = DirectX::XMFLOAT2(3.0f*x, 2.0f*y);

		vertices[4].pos = DirectX::XMFLOAT3(hs, hs, -hs);
		vertices[4].tex = DirectX::XMFLOAT2(4.0f*x, 2.0f*y);

		vertices[5].pos = DirectX::XMFLOAT3(hs, -hs, -hs);
		vertices[5].tex = DirectX::XMFLOAT2(0.0f*x, 1.0f*y);

		vertices[6].pos = DirectX::XMFLOAT3(hs, -hs, hs);
		vertices[6].tex = DirectX::XMFLOAT2(1.0f*x, 1.0f*y);

		vertices[7].pos = DirectX::XMFLOAT3(-hs, -hs, hs);
		vertices[7].tex = DirectX::XMFLOAT2(2.0f*x, 1.0f*y);

		vertices[8].pos = DirectX::XMFLOAT3(-hs, -hs, -hs);
		vertices[8].tex = DirectX::XMFLOAT2(3.0f*x, 1.0f*y);

		vertices[9].pos = DirectX::XMFLOAT3(hs, -hs, -hs);
		vertices[9].tex = DirectX::XMFLOAT2(4.0f*x, 1.0f*y);

		vertices[10].pos = DirectX::XMFLOAT3(-hs, hs, hs);
		vertices[10].tex = DirectX::XMFLOAT2(3.0f*x, 3.0f*y);

		vertices[11].pos = DirectX::XMFLOAT3(hs, hs, hs);
		vertices[11].tex = DirectX::XMFLOAT2(4.0f*x, 3.0f*y);

		vertices[12].pos = DirectX::XMFLOAT3(-hs, -hs, hs);
		vertices[12].tex = DirectX::XMFLOAT2(3.0f*x, 0.0f*y);

		vertices[13].pos = DirectX::XMFLOAT3(hs, -hs, hs);
		vertices[13].tex = DirectX::XMFLOAT2(4.0f*x, 0.0f*y);

		std::vector<unsigned short> indices =
		{
			// front
			9,4,3,
			9,3,8,
			// right
			6,1,0,
			6,0,5,
			// left
			8,3,2,
			8,2,7,
			// bottom
			13,9,8,
			13,8,12,
			// top
			4,11,10,
			4,10,3,
			// back
			7,2,1,
			7,1,6
		};
		return { std::move(vertices),std::move(indices) };

	}
	
	
private:
};
