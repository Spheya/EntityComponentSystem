#pragma once

#include <memory>

#include <Core/Resource.hpp>

#include "Model.hpp"

namespace renderer {
	class ModelResource : public Resource {

	public:
		ModelResource(std::string path) : Resource(path) {}

		void load();

		Model* getModel();
		const Model* getModel() const;

	private:
		std::unique_ptr<Model> _model;

	};
}