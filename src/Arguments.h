#pragma once

#include "IndentableStream.h"

class SyntaxFactory : MSyntax
{
public:
	SyntaxFactory();
	~SyntaxFactory();

	const char* usage() const
	{
		const auto result = m_usage.c_str();
		return result;
	}

	const char* longArgName(const char* shortName) const
	{
		const auto longName = m_argNames.at(shortName);
		return longName;
	}

	static const SyntaxFactory& get();
	static MSyntax createSyntax();

private:
	std::map<const char*, const char*> m_argNames;
	std::string m_usage;

	void registerFlag(std::stringstream& ss, const char *shortName, const char *longName, bool isMultiUse, MArgType argType1 = kNoArg);
	void registerFlag(std::stringstream& ss, const char *shortName, const char *longName, MArgType argType1 = kNoArg);
};

struct AnimClipArg
{
	AnimClipArg(const std::string& name, const MTime& startTime, const MTime& endTime, const double framesPerSecond)
		: name{ name }
		, startTime{ startTime }
		, endTime{ endTime }
		, framesPerSecond{ framesPerSecond }
	{
	}

	AnimClipArg(const AnimClipArg& other)
		: name{ other.name }
		, startTime{ other.startTime }
		, endTime{ other.endTime }
		, framesPerSecond{ other.framesPerSecond }
	{
	}

	AnimClipArg(AnimClipArg&& other) noexcept
		: name{ std::move(other.name) }
		, startTime{ other.startTime }
		, endTime{ other.endTime }
		, framesPerSecond{ other.framesPerSecond }
	{
	}

	AnimClipArg& operator=(AnimClipArg other)
	{
		using std::swap;
		swap(*this, other);
		return *this;
	}

	const std::string name;
	const MTime startTime;
	const MTime endTime;
	const double framesPerSecond;

	MTime duration() const
	{
		return endTime - startTime;
	}

	int frameCount() const
	{
		return static_cast<int>(ceil(duration().as(MTime::kSeconds) * framesPerSecond));
	}
};

class Arguments
{
public:
	Arguments(const MArgList& args, const MSyntax& syntax);
	~Arguments();

	MString sceneName;
	MString outputFolder;
	MSelectionList selection;
	bool glb = false;

	/** Create a default material for primitives that don't have shading in Maya? */
	bool defaultMaterial = false;

	/** Assign a color with a different hue to each material, for debugging purposes */
	bool colorizeMaterials = false;

	/** Convert any material into the scene to a glTF PBR material */
	bool forcePbrMaterials = false;

	/** Always use 32-bit indices, even when 16-bit would be sufficient */
	bool force32bitIndices = false;

	/** If non-null, dump the Maya intermediate objects to the stream */
	IndentableStream* dumpMaya;

	/** If non-null, dump the GLTF JSON to the stream */
	IndentableStream* dumpGLTF;

	/** By default a single GLTF file is exported; pass -separate to move the binary buffers to a separate BIN file */
	bool separate = false;

	/** By default the Maya node name is not assigned to the GLTF node name */
	bool assignObjectNames = false;

	/** Generate debug tangent vector lines? */
	bool debugTangentVectors = false;

	/** Generate debug normal vector lines? */
	bool debugNormalVectors = false;

	/** The length of the debugging vectors */
	float debugVectorLength = 0.1f;

	/** When non-0, instead of using Maya's tangents, use tangents as computed in Morten Mikkelsen's thesis http://image.diku.dk/projects/media/morten.mikkelsen.08.pdf*/
	float mikkelsenTangentAngularThreshold = 0;

	/** The scale factor to apply to the vertex positions */
	float scaleFactor = 1;

	/** The opacity factor to apply to the material */
	float opacityFactor = 1;

	/** The time where the 'initial values' of all nodes are to be found (aka neutral base pose) */
	MTime initialValuesTime;

	/** Redraw the viewport while exporting? True in debug builds by default, false otherwise (for speed) */
#ifdef _DEBUG
	bool redrawViewport = true;
#else
	bool redrawViewport = false;
#endif

	std::vector<AnimClipArg> animationClips;

private:
	std::ofstream m_mayaOutputFileStream;
	std::ofstream m_gltfOutputFileStream;

	std::unique_ptr<IndentableStream> m_mayaOutputStream;
	std::unique_ptr<IndentableStream> m_gltfOutputStream;
};

