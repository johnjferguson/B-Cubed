#include "Mesh.h"
#include "assimp/include/Importer.hpp"
#include "assimp/include/scene.h"
#include "assimp/include/postprocess.h"
#include "BindableHeader.h"

namespace dx = DirectX;

Mesh::Mesh(Graphics & gfx, float scale, const std::string & path)
{
	struct Vertex
	{
		dx::XMFLOAT3 pos;
		dx::XMFLOAT3 n;
		dx::XMFLOAT2 tex;
	};

	Assimp::Importer imp;
	auto pModel = imp.ReadFile(path.c_str(), 
		aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
	);


	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	std::vector<DirectX::XMFLOAT2> temp = 
	{
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};

	for (int i = 0; i < pModel->mNumMeshes; i++)
	{
		const auto pMesh = pModel->mMeshes[i];
		int k = vertices.size();

		for (unsigned int j = 0; j < pMesh->mNumVertices; j++)
		{
			if (!(path == "models//vehicle.obj" && i == 1))
			{
				vertices.push_back(
					{
						{pMesh->mVertices[j].x*scale, pMesh->mVertices[j].y*scale, pMesh->mVertices[j].z*scale},
						*reinterpret_cast<dx::XMFLOAT3*>(&pMesh->mNormals[j]),
						*reinterpret_cast<dx::XMFLOAT2*>(&pMesh->mTextureCoords[0][j])

					}
				);
			}
			// need to flip the y so the texture maps properly
			vertices.back().tex.y = 1 - vertices.back().tex.y;
			vertices.back().n.x = -vertices.back().n.x;
			vertices.back().n.y = -vertices.back().n.y;
			vertices.back().n.z = -vertices.back().n.z;

			// add all the vertex point to renderable
			m_vertices.push_back({ pMesh->mVertices[j].x *scale, pMesh->mVertices[j].y*scale, pMesh->mVertices[j].z * scale });
		}
		for (unsigned int j = 0; j < pMesh->mNumFaces; j++)
		{
			const auto& face = pMesh->mFaces[j];
			if (face.mNumIndices == 3)
			{
				indices.push_back(k + face.mIndices[0]);
				indices.push_back(k + face.mIndices[1]);
				indices.push_back(k + face.mIndices[2]);

				indices.push_back(k + face.mIndices[0]);
				indices.push_back(k + face.mIndices[2]);
				indices.push_back(k + face.mIndices[1]);
			}
		}
	}


	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));
	AddBind(std::make_unique<IndexBuffer>(gfx, indices));

	// add indices to renderable
	m_indices = std::move(indices);

	if (path == "models//vehicle.obj")
		//AddBind(std::make_unique<Texture>(gfx, L"images//color.jpg"));
		AddBind(std::make_unique<Texture>(gfx, L"images//vehicle.png"));
	else if (path == "models//finalfinalwall.obj")
		AddBind(std::make_unique<Texture>(gfx, L"images//texture_5.png"));
	else if (path == "models//secondary.obj")
		AddBind(std::make_unique<Texture>(gfx, L"images//withpink.png"));
	else if (path == "models//SPLINE2old.obj")
		AddBind(std::make_unique<Texture>(gfx, L"images//green.jpg"));
	else if (path == "models//background.obj")
		AddBind(std::make_unique<Texture>(gfx, L"images//background.png"));
	else
		AddBind(std::make_unique<Texture>(gfx, L"images//withpink.png"));

	std::unique_ptr<VertexShader> pVertexShader = std::make_unique<VertexShader>(gfx, L"VertexShaderTexture.cso");
	ID3DBlob* vertexBlob = pVertexShader->GetBlob();
	AddBind(std::move(pVertexShader));

	AddBind(std::make_unique<PixelShader>(gfx, L"PixelShaderTexture.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};
	AddBind(std::make_unique<InputLayout>(gfx, vertexBlob, ied));

	struct VertexConstant
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX perspective;
		DirectX::XMMATRIX lightView;
		DirectX::XMMATRIX lightProjection;
		DirectX::XMFLOAT4 lightPosition;
	}vertexConstant;

	struct Effect
	{
		int effect;
		int j[3];
	}effect;

	// constant buffers
	AddBind(std::make_unique<VertexConstantBuffer>(gfx, vertexConstant));
	AddBind(std::make_unique<PixelConstantBuffer>(gfx, effect));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	AddBind(std::make_unique<Sampler>(gfx));

	AddDepthBind(std::make_unique<VertexBuffer>(gfx, vertices));

	std::unique_ptr<VertexShader> pVertexShaderDepth = std::make_unique<VertexShader>(gfx, L"VertexShaderDepth.cso");
	ID3DBlob* vertexBlobDepth = pVertexShaderDepth->GetBlob();
	AddDepthBind(std::move(pVertexShaderDepth));

	AddDepthBind(std::make_unique<PixelShader>(gfx, L"PixelShaderDepth.cso"));

	std::vector<D3D11_INPUT_ELEMENT_DESC> ied0 =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	AddDepthBind(std::make_unique<InputLayout>(gfx, vertexBlobDepth, ied0));

	AddDepthBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

}
