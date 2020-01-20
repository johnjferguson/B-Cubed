#include "Topology.h"

Topology::Topology(Graphics & gfx, D3D11_PRIMITIVE_TOPOLOGY topology)
	:
	topology(topology)
{}

void Topology::Bind(Graphics & gfx)
{
	gfx.GetContext()->IASetPrimitiveTopology(topology);
}
