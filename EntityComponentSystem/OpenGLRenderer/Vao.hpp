#pragma once
#include <GL/glew.h>
#include "Vbo.hpp"

namespace renderer {
	class Vao
	{
	public:
		Vao();
		Vao(Vao&) = delete;
		Vao& operator=(Vao&) = delete;
		Vao(Vao&& other) noexcept;
		Vao& operator=(Vao&& other) noexcept;
		~Vao();

		template<typename ... T>
		size_t createVbo(const std::vector<GLuint>& indices, GLenum usage);

		template<typename ... T>
		Vbo<T...>& getVbo(size_t id);

		[[nodiscard]] bool isValid() const;

		void bind() const;
		static void unbind();

		void prepareDraw() const;
		void finishDraw() const;

	private:
		GLuint _id;

		std::vector<GLuint> _attributes;
		std::vector<IVbo*> _vbos;
	};

	template <typename ... T>
	size_t Vao::createVbo(const std::vector<GLuint>& indices, GLenum usage) {
		_attributes.reserve(_attributes.size() + indices.size());
		for (GLuint i : indices) {
			assert(std::find(_attributes.begin(), _attributes.end(), i) == _attributes.end());
			_attributes.push_back(i);
		}

		_vbos.push_back(new Vbo<T...>(indices, usage));
		return _vbos.size() - 1;
	}

	template <typename ... T>
	Vbo<T...>& Vao::getVbo(size_t id) {
		return *reinterpret_cast<Vbo<T...>*>(_vbos[id]);
	}
}
