#pragma once

#include "sceneTypes.h"
#include "MeshRenderables.h"
#include "ExportableMesh.h"

typedef std::vector<std::unique_ptr<GLTF::Accessor>> MeshAccessorPerSetIndex;
typedef std::array<MeshAccessorPerSetIndex, Semantic::COUNT> MeshAccessorPerSetIndexTable;

class ExportableResources;

class ExportablePrimitive
{
public:
	ExportablePrimitive(const VertexBuffer& vertexBuffer, ExportableResources& resources);
	
	ExportablePrimitive(const VertexBuffer& vertexBuffer, ExportableResources& resources, 
		const Semantic::Kind debugSemantic, const double debugLineLength, const Color debugLineColor);
	
	virtual ~ExportablePrimitive();

	GLTF::Primitive glPrimitive;
	std::unique_ptr<GLTF::Accessor> glIndices;
	MeshAccessorPerSetIndexTable glAccessorTable;
};
