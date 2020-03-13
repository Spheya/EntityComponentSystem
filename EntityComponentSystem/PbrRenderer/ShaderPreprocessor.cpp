#include "ShaderPreprocessor.hpp"

#include <unordered_map>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace {
	std::string processFile(std::string shaderFile, std::string path, bool instanced, std::unordered_map<std::string, bool>& includedFiles) {
		// Get the absolute path to this file
		std::filesystem::path p = path;
		std::string absolutePath = std::filesystem::absolute(p).u8string();
		
		// If the file is already included, do nothing
		if (includedFiles[absolutePath])
			return "";

		// Mark the file as included
		includedFiles[absolutePath] = true;

		// Read all the text in the file
		std::ifstream fileStream(shaderFile);
		std::ostringstream processedCode;

		std::string line;
		while (std::getline(fileStream, line)) {
			std::istringstream iss(line);
			std::string statement;
			iss >> statement;
			if (statement == "#include") {
				iss >> statement;
				std::string relativeFilePath = statement.substr(1, statement.size() - 2);
				processedCode << processFile(relativeFilePath, absolutePath + relativeFilePath, instanced, includedFiles);
			} else {
				processedCode << line;
				processedCode << '\n';
			}
		}

		return processedCode.str();
	}
}

std::string renderer::ShaderPreprocessor::process(std::string shaderFile) {
	std::unordered_map<std::string, bool> includedFiles;

	bool instanced = false;

	std::istringstream shaderCode(processFile(shaderFile, std::filesystem::current_path().u8string() + shaderFile, instanced, includedFiles));
	std::ostringstream processedCode;

	int scope = 0;

	std::string line;
	while (getline(shaderCode, line)) {
		std::istringstream iss(line);
		std::string statement;
		iss >> statement;
		for (auto c : statement) {
			if (c == '{') {
				++scope;
			} else if(c == '}') {
				--scope;
			}
		}

		if (scope == 0 && statement == "instance") {
			std::string remaining;
			getline(iss, remaining);
			processedCode << (instanced ? "attribute" : "uniform") << remaining;
		} else {
			processedCode << line;
			processedCode << '\n';
		}
	}

	return processedCode.str();
}
