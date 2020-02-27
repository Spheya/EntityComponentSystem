#pragma once
#include "Vao.h"

#include <functional>

namespace renderer {
	class Model {
	public:
		explicit Model(size_t nVertices, GLenum drawMode);

		void bindVao() const;

		template<typename ... T>
		Vbo<T...>& getVbo(size_t index);

		template<typename T>
		size_t createVbo(size_t index, GLenum usage);

		template<typename ... T>
		size_t createVbo(const std::vector<GLuint>& indices, GLenum usage);

		void prepareDraw() const;
		void draw() const;
		void finishDraw() const;

		void setDrawMode(GLenum drawMode);
		[[nodiscard]] GLenum getDrawMode() const;

		void setVerticesAmount(size_t nVertices);
		[[nodiscard]] size_t getVerticesAmount() const;

	private:
		Vao m_vao;
		size_t m_nVertices;
		GLenum m_drawMode;
	};

	template <typename ... T>
	Vbo<T...>& Model::getVbo(size_t index) {
		return m_vao.getVbo<T...>(index);
	}

	template <typename T>
	size_t Model::createVbo(size_t index, GLenum usage) {
		return createVbo<T>(std::vector<GLuint>{GLuint(index)}, usage);
	}

	template <typename ... T>
	size_t Model::createVbo(const std::vector<GLuint>& indices, GLenum usage) {
		return m_vao.createVbo<T...>(indices, usage);
	}
}

