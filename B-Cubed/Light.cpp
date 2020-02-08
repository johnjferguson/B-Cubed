#include "Light.h"
#include "Gui.h"

Light::Light(Graphics& gfx, const DirectX::XMFLOAT4 & pos)
	:
	pos(pos),
	lightBox(gfx, 0.25f)
{}

const DirectX::XMFLOAT4 & Light::GetPosition() const
{
	return pos;
}

void Light::Update(Graphics& gfx, const DirectX::XMMATRIX& camera)
{
	Gui::AddSlider("Light X", pos.x, -40.0f, 40.0f);
	Gui::AddSlider("Light Y", pos.y, -40.0f, 40.0f);
	Gui::AddSlider("Light Z", pos.z, -40.0f, 40.0f);

	struct VertexConstant
	{
		DirectX::XMMATRIX transform;
		DirectX::XMMATRIX perspective;
	};

	VertexConstant vc
	{
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z)*
		camera,
		DirectX::XMMatrixPerspectiveLH(1.0f, float(gfx.GetHeight()) / float(gfx.GetWidth()), 0.5f, 400.0f)
	};
	lightBox.UpdateVertex(gfx, vc);
}

void Light::Render(Graphics & gfx)
{
	lightBox.Render(gfx);
}

void Light::SetPosition(const DirectX::XMFLOAT4 & newPos)
{
	pos = newPos;
}

DirectX::XMMATRIX Light::LookAt(const DirectX::XMFLOAT3& target) const
{
	DirectX::XMVECTOR look = DirectX::XMVectorSubtract(DirectX::XMVectorSet(target.x, target.y, target.z, 0.0f),
													   DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f));
	look = DirectX::XMVector3Normalize(look);

	return DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(pos.x, pos.y, pos.z, pos.w), look, DirectX::XMVectorSet(0.0f,1.0f,0.0f,0.0f));
}
