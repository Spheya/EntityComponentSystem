#pragma once
#include <GL/glew.h>
#include "Vbo.h"

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
		GLuint m_id;

		std::vector<GLuint> m_attributes;
		std::vector<IVbo*> m_vbos;
	};

	template <typename ... T>
	size_t Vao::createVbo(const std::vector<GLuint>& indices, GLenum usage) {
		m_attributes.reserve(m_attributes.size() + indices.size());
		for (GLuint i : indices) {
			assert(std::find(m_attributes.begin(), m_attributes.end(), i) == m_attributes.end());
			m_attributes.push_back(i);
		}

		m_vbos.push_back(new Vbo<T...>(indices, usage));
		return m_vbos.size() - 1;
	}

	template <typename ... T>
	Vbo<T...>& Vao::getVbo(size_t id) {
		return *reinterpret_cast<Vbo<T...>*>(m_vbos[id]);
	}
}
