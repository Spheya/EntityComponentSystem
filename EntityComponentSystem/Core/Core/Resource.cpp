#include "Resource.hpp"

#include <filesystem>

Resource::Resource(const std::string& path) :
	_path(path)
{
	if (path.empty())
		return;

	std::filesystem::path p = path;
	_filename = p.filename().u8string();
	_fileExtension = p.extension().u8string();
}

const std::string& Resource::getFilename() const {
	return _filename;
}

const std::string& Resource::getFileExtension() const {
	return _fileExtension;
}

const std::string& Resource::getPath() const {
	return _path;
}
