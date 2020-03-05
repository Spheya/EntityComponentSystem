#pragma once

#include <array>

#include "ShaderProgram.hpp"

namespace renderer {

	class InstanceData {
		struct IUniform {
			virtual void bind(ShaderProgram& program, const std::string& name) const = 0;
			virtual void addToInstancedVector(std::vector<uint8_t>& vec) const = 0;
		};

		template<typename T>
		struct Uniform : public IUniform {
			void bind(ShaderProgram& program, const std::string& name) const override;
			void addToInstancedVector(std::vector<uint8_t>& vec) const override;
			T data;
		};

	public:
		void bindUniforms(ShaderProgram& program);
		void addToInstancedVector(std::vector<uint8_t>& vec, const std::vector<std::string> order);

		template<typename T>
		void store(const std::string& name, T&& value);

		template<typename T>
		void store(const std::string& name, const T& value);
	private:
		std::unordered_map<std::string, std::unique_ptr<IUniform>> _uniforms;
	};


	template<typename T>
	void InstanceData::Uniform<T>::addToInstancedVector(std::vector<uint8_t>& vec) const {
		const uint8_t* begin = reinterpret_cast<const uint8_t*>(&data);
		const uint8_t* end = begin + sizeof(T);
		
		for (const auto* it = begin; begin != end; ++it)
			vec.push_back(*it);
	}

	template<typename T>
	inline void InstanceData::store(const std::string& name, T&& value) {
		auto data = dynamic_cast<Uniform<T>*>(_uniforms[name].get());
		data->data = std::move(value);
	}

	template<typename T>
	inline void InstanceData::store(const std::string& name, const T& value) {
		auto data = dynamic_cast<Uniform<T>*>(_uniforms[name].get());
		data->data = value;
	}

}