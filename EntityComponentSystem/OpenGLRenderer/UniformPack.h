#pragma once
#include <GL/glew.h>
#include <vector>

namespace renderer {
	class UniformPack {
	public:

		// TODO: Add more types, although they are useless in most situations
		enum class UniformType {
			_1F, _2F, _3F, _4F,
			_1I, _2I, _3I, _4I,

			MATRIX2F, MATRIX3F, MATRIX4F
		};

		bool remove(GLint id);
		[[nodiscard]] bool contains(GLint id) const;
		void set(GLint id, const void* data, UniformType type, GLsizei count = 1);
		void apply() const;

	private:
		struct Uniform {
			Uniform(GLint position, const void* data, UniformType type, GLsizei count) :
				position(position),
				data(data),
				type(type),
				count(count)
			{}

			GLint position;
			const void* data;
			UniformType type;
			GLsizei count;
		};

		std::vector<Uniform> m_uniforms;
	};
}