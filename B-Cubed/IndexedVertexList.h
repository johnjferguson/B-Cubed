#pragma once
#include <vector>

template <class V>
class IndexedVertexList
{
public:
	IndexedVertexList() = default;
	std::vector<V> vertices;
	std::vector<unsigned short> indices;
};
