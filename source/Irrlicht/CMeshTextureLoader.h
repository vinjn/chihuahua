#ifndef IRR_C_MESH_TEXTURE_LOADER_H_INCLUDED
#define IRR_C_MESH_TEXTURE_LOADER_H_INCLUDED

#include "IMeshTextureLoader.h"

namespace ue
{
namespace io
{
	class IFileSystem;
} // end namespace io
namespace video
{
	class IVideoDriver;
}

namespace scene
{

class CMeshTextureLoader : public IMeshTextureLoader
{
public:
	CMeshTextureLoader(ue::io::IFileSystem* fs, ue::video::IVideoDriver* driver);

	//! Set a custom texture path.
    /**	This is the first path the texture-loader should search.  */
	virtual void setTexturePath(const ue::io::path& path)  _IRR_OVERRIDE_;

	//! Get the current custom texture path.
	virtual const ue::io::path& getTexturePath() const  _IRR_OVERRIDE_;

	//! Get the texture by searching for it in all paths that makes sense for the given textureName.
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
	\param textureName Texturename as used in the mesh-format
	\return Pointer to the texture. Returns 0 if loading failed.*/
	virtual ue::video::ITexture* getTexture(const ue::io::path& textureName)  _IRR_OVERRIDE_;

	//! Meshloaders will search paths relative to the meshFile.
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
		Any values you set here will likely be overwritten internally. */
	virtual void setMeshFile(const ue::io::IReadFile* meshFile) _IRR_OVERRIDE_;

	//! Meshloaders will try to look relative to the path of the materialFile
	/** Usually you do not have to use this method, it is used internally by IMeshLoader's.
	Any values you set here will likely be overwritten internally.	*/
	virtual void setMaterialFile(const ue::io::IReadFile* materialFile) _IRR_OVERRIDE_;

protected:
	// make certain path's have a certain internal format
	void preparePath(ue::io::path& directory)
	{
		if (!directory.empty())
		{
			if (directory == _IRR_TEXT("."))
				directory = _IRR_TEXT("");

			directory.replace(_IRR_TEXT('\\'),_IRR_TEXT('/'));
			if (directory.lastChar() != _IRR_TEXT('/'))
				directory.append(_IRR_TEXT('/'));
		}
	}

	// Save the texturename when it's a an existing file
	bool checkTextureName( const ue::io::path& filename);

private:
	ue::io::IFileSystem * FileSystem;
	ue::video::IVideoDriver* VideoDriver;
	ue::io::path TexturePath;
	const ue::io::IReadFile* MeshFile;
	ue::io::path MeshPath;
	const ue::io::IReadFile* MaterialFile;
	ue::io::path MaterialPath;
	ue::io::path TextureName;
};

} // end namespace scene
} // end namespace ue

#endif

