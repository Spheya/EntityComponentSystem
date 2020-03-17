#include "ModelResource.hpp"

void renderer::ModelResource::load() {
	_model = std::make_unique<Model>(getPath());
}

renderer::Model* renderer::ModelResource::getModel() {
	return _model.get();
}

const renderer::Model* renderer::ModelResource::getModel() const {
	return _model.get();
}
