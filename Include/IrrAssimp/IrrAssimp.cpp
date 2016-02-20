#include "IrrAssimp.h"

#include <iostream>

using namespace irr;

IrrAssimp::IrrAssimp(irr::scene::ISceneManager* smgr) : Smgr(smgr), Cache(smgr->getMeshCache()), FileSystem(smgr->getFileSystem()), Importer(smgr), Exporter()
{

}

IrrAssimp::~IrrAssimp()
{

}

bool IrrAssimp::exportMesh(irr::scene::IMesh* mesh, irr::core::stringc format, irr::core::stringc path)
{
    Exporter.writeFile(mesh, format, path);
    return true;
}

irr::scene::IAnimatedMesh* IrrAssimp::getMesh(const io::path& path)
{
	scene::IAnimatedMesh* msh = Cache->getMeshByName(path);
	if (msh)
		return msh;

	io::IReadFile* file = FileSystem->createAndOpenFile(path);
	if (!file)
	{
		//os::Printer::log("Could not load mesh, because file could not be opened: ", path, ELL_ERROR);
        throw std::invalid_argument("Path not valid");
		return 0;
	}

	if (isLoadable(path))
    {
        msh = Importer.createMesh(file);

        if (msh)
        {
            Cache->addMesh(path, msh);
            msh->drop();
        }
    }

	file->drop();

    
    if(!msh)
        throw std::invalid_argument("not a valid type");
	return msh;
}

irr::core::stringc IrrAssimp::getError()
{
    return Importer.Error;
}


core::array<ExportFormat> IrrAssimp::getExportFormats()
{
    core::array<ExportFormat> formats;

    Assimp::Exporter exporter;
    for (int i = 0; i < exporter.GetExportFormatCount(); ++i)
    {
         const aiExportFormatDesc* formatDesc = exporter.GetExportFormatDescription(i);
         formats.push_back(ExportFormat(formatDesc->fileExtension, formatDesc->id, formatDesc->description));
    }

    return formats;
}


bool IrrAssimp::isLoadable(irr::io::path path)
{
    return Importer.isALoadableFileExtension(path);
}
