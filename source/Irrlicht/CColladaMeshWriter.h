// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __IRR_C_COLLADA_MESH_WRITER_H_INCLUDED__
#define __IRR_C_COLLADA_MESH_WRITER_H_INCLUDED__

#include "IColladaMeshWriter.h"
#include "S3DVertex.h"
#include "irrMap.h"
#include "IVideoDriver.h"

namespace ue
{
namespace io
{
	class IXMLWriter;
	class IFileSystem;
}

namespace scene
{
	//! Callback interface for properties which can be used to influence collada writing
	// (Implementer note: keep namespace labels here to make it easier for users copying this one)
	class CColladaMeshWriterProperties  : public virtual IColladaMeshWriterProperties
	{
	public:
		//! Which lighting model should be used in the technique (FX) section when exporting effects (materials)
		virtual ue::scene::E_COLLADA_TECHNIQUE_FX getTechniqueFx(const ue::video::SMaterial& material) const _IRR_OVERRIDE_;

		//! Which texture index should be used when writing the texture of the given sampler color.
		virtual ue::s32 getTextureIdx(const ue::video::SMaterial & material, ue::scene::E_COLLADA_COLOR_SAMPLER cs) const _IRR_OVERRIDE_;

		//! Return which color from Irrlicht should be used for the color requested by collada
		virtual ue::scene::E_COLLADA_IRR_COLOR getColorMapping(const ue::video::SMaterial & material, ue::scene::E_COLLADA_COLOR_SAMPLER cs) const _IRR_OVERRIDE_;

		//! Return custom colors for certain color types requested by collada.
		virtual ue::video::SColor getCustomColor(const ue::video::SMaterial & material, ue::scene::E_COLLADA_COLOR_SAMPLER cs) const _IRR_OVERRIDE_;

		//! Return the settings for transparence
		virtual ue::scene::E_COLLADA_TRANSPARENT_FX getTransparentFx(const ue::video::SMaterial& material) const _IRR_OVERRIDE_;

		//! Transparency value for that material.
		virtual ue::f32 getTransparency(const ue::video::SMaterial& material) const _IRR_OVERRIDE_;

		//! Reflectivity value for that material
		virtual ue::f32 getReflectivity(const ue::video::SMaterial& material) const _IRR_OVERRIDE_;

		//! Return index of refraction for that material
		virtual ue::f32 getIndexOfRefraction(const ue::video::SMaterial& material) const _IRR_OVERRIDE_;

		//! Should node be used in scene export? By default all visible nodes are exported.
		virtual bool isExportable(const ue::scene::ISceneNode * node) const _IRR_OVERRIDE_;

		//! Return the mesh for the given nod. If it has no mesh or shouldn't export it's mesh return 0.
		virtual ue::scene::IMesh* getMesh(ue::scene::ISceneNode * node) _IRR_OVERRIDE_;

		//! Return if the node has it's own material overwriting the mesh-materials
		virtual bool useNodeMaterial(const scene::ISceneNode* node) const _IRR_OVERRIDE_;
	};

	class CColladaMeshWriterNames  : public virtual IColladaMeshWriterNames
	{
	public:
		CColladaMeshWriterNames(IColladaMeshWriter * writer);
		virtual ue::core::stringw nameForMesh(const scene::IMesh* mesh, int instance) _IRR_OVERRIDE_;
		virtual ue::core::stringw nameForNode(const scene::ISceneNode* node) _IRR_OVERRIDE_;
		virtual ue::core::stringw nameForMaterial(const video::SMaterial & material, int materialId, const scene::IMesh* mesh, const scene::ISceneNode* node) _IRR_OVERRIDE_;
	protected:
		ue::core::stringw nameForPtr(const void* ptr) const;
	private:
		IColladaMeshWriter * ColladaMeshWriter;
	};



//! class to write meshes, implementing a COLLADA (.dae, .xml) writer
/** This writer implementation has been originally developed for irrEdit and then
merged out to the Irrlicht Engine */
class CColladaMeshWriter : public IColladaMeshWriter
{
public:

	CColladaMeshWriter(ISceneManager * smgr, video::IVideoDriver* driver, io::IFileSystem* fs);
	virtual ~CColladaMeshWriter();

	//! Returns the type of the mesh writer
	virtual EMESH_WRITER_TYPE getType() const _IRR_OVERRIDE_;

	//! writes a scene starting with the given node
	virtual bool writeScene(io::IWriteFile* file, scene::ISceneNode* root) _IRR_OVERRIDE_;

	//! writes a mesh
	virtual bool writeMesh(io::IWriteFile* file, scene::IMesh* mesh, s32 flags=EMWF_NONE) _IRR_OVERRIDE_;

	// Restrict the characters of oldString a set of allowed characters in xs::NCName and add the prefix.
	virtual ue::core::stringw toNCName(const ue::core::stringw& oldString, const ue::core::stringw& prefix=ue::core::stringw(L"_NC_")) const _IRR_OVERRIDE_;

	//! After export you can find out which name had been used for writing the geometry for this node.
	virtual const ue::core::stringw* findGeometryNameForNode(ISceneNode* node) _IRR_OVERRIDE_;

protected:

	void reset();
	bool hasSecondTextureCoordinates(video::E_VERTEX_TYPE type) const;
	void writeUv(const ue::core::vector2df& vec);
	void writeVector(const ue::core::vector2df& vec);
	void writeVector(const ue::core::vector3df& vec);
	void writeColor(const ue::video::SColorf& colorf, bool writeAlpha=true);
	inline ue::core::stringw toString(const ue::video::ECOLOR_FORMAT format) const;
	inline ue::core::stringw toString(const ue::video::E_TEXTURE_CLAMP clamp) const;
	inline ue::core::stringw toString(const ue::scene::E_COLLADA_TRANSPARENT_FX opaque) const;
	inline ue::core::stringw toRef(const ue::core::stringw& source) const;
	bool isCamera(const scene::ISceneNode* node) const;
	ue::core::stringw nameForMesh(const scene::IMesh* mesh, int instance) const;
	ue::core::stringw nameForNode(const scene::ISceneNode* node) const;
	ue::core::stringw nameForMaterial(const video::SMaterial & material, int materialId, const scene::IMesh* mesh, const scene::ISceneNode* node);
	ue::core::stringw nameForMaterialSymbol(const scene::IMesh* mesh, int materialId) const;
	ue::core::stringw findCachedMaterialName(const ue::video::SMaterial& material) const;
	ue::core::stringw minTexfilterToString(bool bilinear, bool trilinear) const;
	ue::core::stringw magTexfilterToString(bool bilinear, bool trilinear) const;
	ue::core::stringw pathToURI(const ue::io::path& path) const;
	inline bool isXmlNameStartChar(wchar_t c) const;
	inline bool isXmlNameChar(wchar_t c) const;
	s32 getCheckedTextureIdx(const video::SMaterial & material, E_COLLADA_COLOR_SAMPLER cs);
	video::SColor getColorMapping(const video::SMaterial & material, E_COLLADA_COLOR_SAMPLER cs, E_COLLADA_IRR_COLOR colType);
	void writeAsset();
	void makeMeshNames(ue::scene::ISceneNode * node);
	void writeNodeMaterials(ue::scene::ISceneNode * node);
	void writeNodeEffects(ue::scene::ISceneNode * node);
	void writeNodeLights(ue::scene::ISceneNode * node);
	void writeNodeCameras(ue::scene::ISceneNode * node);
	void writeAllMeshGeometries();
	void writeSceneNode(ue::scene::ISceneNode * node);
	void writeMeshMaterials(scene::IMesh* mesh, ue::core::array<ue::core::stringw> * materialNamesOut=0);
	void writeMeshEffects(scene::IMesh* mesh);
	void writeMaterialEffect(const ue::core::stringw& materialname, const video::SMaterial & material);
	void writeMeshGeometry(const ue::core::stringw& meshname, scene::IMesh* mesh);
	void writeMeshInstanceGeometry(const ue::core::stringw& meshname, scene::IMesh* mesh, scene::ISceneNode* node=0);
	void writeMaterial(const ue::core::stringw& materialname);
	void writeLightInstance(const ue::core::stringw& lightName);
	void writeCameraInstance(const ue::core::stringw& cameraName);
	void writeLibraryImages();
	void writeColorFx(const video::SMaterial & material, const wchar_t * colorname, E_COLLADA_COLOR_SAMPLER cs, const wchar_t* attr1Name=0, const wchar_t* attr1Value=0);
	void writeAmbientLightElement(const video::SColorf & col);
	void writeColorElement(const video::SColor & col, bool writeAlpha=true);
	void writeColorElement(const video::SColorf & col, bool writeAlpha=true);
	void writeTextureSampler(s32 textureIdx);
	void writeFxElement(const video::SMaterial & material, E_COLLADA_TECHNIQUE_FX techFx);
	void writeNode(const wchar_t * nodeName, const wchar_t * content);
	void writeFloatElement(ue::f32 value);
	void writeRotateElement(const ue::core::vector3df& axis, ue::f32 angle);
	void writeScaleElement(const ue::core::vector3df& scale);
	void writeTranslateElement(const ue::core::vector3df& translate);
	void writeLookAtElement(const ue::core::vector3df& eyePos, const ue::core::vector3df& targetPos, const ue::core::vector3df& upVector);
	void writeMatrixElement(const ue::core::matrix4& matrix);

	struct SComponentGlobalStartPos
	{
		SComponentGlobalStartPos() : PosStartIndex(-1), PosLastIndex(-1),
				NormalStartIndex(-1), NormalLastIndex(-1),
				TCoord0StartIndex(-1), TCoord0LastIndex(-1),
				TCoord1StartIndex(-1), TCoord1LastIndex(-1)
		{ }

		s32 PosStartIndex;
		s32 PosLastIndex;

		s32 NormalStartIndex;
		s32 NormalLastIndex;

		s32 TCoord0StartIndex;
		s32 TCoord0LastIndex;

		s32 TCoord1StartIndex;
		s32 TCoord1LastIndex;
	};

	io::IFileSystem* FileSystem;
	video::IVideoDriver* VideoDriver;
	io::IXMLWriter* Writer;
	core::array<video::ITexture*> LibraryImages;
	io::path Directory;

	// Helper struct for creating geometry copies for the ECGI_PER_MESH_AND_MATERIAL settings.
	struct SGeometryMeshMaterials
	{
		bool equals(const core::array<ue::core::stringw>& names) const
		{
			if ( names.size() != MaterialNames.size() )
				return false;
			for ( ue::u32 i=0; i<MaterialNames.size(); ++i )
				if ( names[i] != MaterialNames[i] )
					return false;
			return true;
		}

		ue::core::stringw GeometryName;				// replacing the usual ColladaMesh::Name
		core::array<ue::core::stringw> MaterialNames;	// Material names exported for this instance
		core::array<const ISceneNode*> MaterialOwners;	// Nodes using this specific mesh-material combination
	};

	// Check per mesh-ptr if stuff has been written for this mesh already
	struct SColladaMesh
	{
		SColladaMesh() : MaterialsWritten(false), EffectsWritten(false)
		{
		}

		SGeometryMeshMaterials * findGeometryMeshMaterials(const ue::core::array<ue::core::stringw> materialNames)
		{
			for ( ue::u32 i=0; i<GeometryMeshMaterials.size(); ++i )
			{
				if ( GeometryMeshMaterials[i].equals(materialNames) )
					return &(GeometryMeshMaterials[i]);
			}
			return NULL;
		}

		const ue::core::stringw& findGeometryNameForNode(const ISceneNode* node) const
		{
			if ( GeometryMeshMaterials.size() < 2 )
				return Name;
			for ( ue::u32 i=0; i<GeometryMeshMaterials.size(); ++i )
			{
				if ( GeometryMeshMaterials[i].MaterialOwners.linear_search(node)  >= 0 )
					return GeometryMeshMaterials[i].GeometryName;
			}
			return Name; // (shouldn't get here usually)
		}

		ue::core::stringw Name;
		bool MaterialsWritten;	// just an optimization doing that here in addition to the MaterialsWritten map
		bool EffectsWritten;	// just an optimization doing that here in addition to the EffectsWritten map

		core::array<SGeometryMeshMaterials> GeometryMeshMaterials;
	};
	typedef core::map<IMesh*, SColladaMesh>::Node MeshNode;
	core::map<IMesh*, SColladaMesh> Meshes;

	// structure for the lights library
	struct SColladaLight
	{
		SColladaLight()	{}
		ue::core::stringw Name;
	};
	typedef core::map<ISceneNode*, SColladaLight>::Node LightNode;
	core::map<ISceneNode*, SColladaLight> LightNodes;

	// structure for the camera library
	typedef core::map<ISceneNode*, ue::core::stringw>::Node CameraNode;
	core::map<ISceneNode*, ue::core::stringw> CameraNodes;

	// Check per name if stuff has been written already
	// TODO: second parameter not needed, we just don't have a core::set class yet in Irrlicht
	core::map<ue::core::stringw, bool> MaterialsWritten;
	core::map<ue::core::stringw, bool> EffectsWritten;

	// Cache material names
	struct MaterialName
	{
		MaterialName(const ue::video::SMaterial & material, const ue::core::stringw& name)
			: Material(material), Name(name)
		{}
		ue::video::SMaterial Material;
		ue::core::stringw Name;
	};
	ue::core::array< MaterialName > MaterialNameCache;
};


} // end namespace
} // end namespace

#endif
