#pragma once

#include <string>

class Resource {
public:
	explicit Resource(const std::string& path);
	virtual ~Resource() = default;

	virtual void load() = 0;

	const std::string& getFilename() const;
	const std::string& getFileExtension() const;
	const std::string& getPath() const;

private:
	std::string _filename;
	std::string _fileExtension;
	std::string _path;
};

