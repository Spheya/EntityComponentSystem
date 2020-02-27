#pragma once

#include <GL/glew.h>
#include <memory>
#include <vector>
#include <tuple>
#include <cassert>
#include <array>
#include <glm/vec3.hpp>
#include <iostream>

namespace renderer {

	template<typename T, size_t Size>
	class VboElementData {
	public:
		using TYPE = T;
		constexpr static size_t SIZE = Size;

		VboElementData(std::initializer_list<T> init) {
			assert(init.size() == Size);
			for (int i = 0; i < Size; ++i)
				_data[i] = *(init.begin() + i);
		}

		T& operator[](size_t index);
		const T& operator[](size_t index) const;

		constexpr static GLenum getType();

	private:
		T _data[Size];
	};

	template <typename T, size_t Size>
	T& VboElementData<T, Size>::operator[](size_t index) {
		assert(index < Size);
		return _data[index];
	}

	template <typename T, size_t Size>
	const T& VboElementData<T, Size>::operator[](size_t index) const {
		assert(index < Size);
		return _data[index];
	}

	template <typename T, size_t Size>
	constexpr GLenum VboElementData<T, Size>::getType() {
		if constexpr (std::is_same_v<T, GLfloat>) {
			return GL_FLOAT;
		} else if constexpr (std::is_same_v<T, GLint>) {
			return GL_INT;
		} else if constexpr (std::is_same_v<T, GLuint>) {
			return GL_UNSIGNED_INT;
		} else if constexpr (std::is_same_v<T, GLboolean>) {
			return GL_BOOL;
		} else if constexpr (std::is_same_v<T, GLdouble>) {
			return GL_DOUBLE;
		} else {
			std::terminate();
		}
	}
	
	namespace element {
		using Float = VboElementData<GLfloat, 1>;
		using Int = VboElementData<GLint, 1>;
		using Uint = VboElementData<GLuint, 1>;
		using Double = VboElementData<GLdouble, 1>;
		using Boolean = VboElementData<GLboolean, 1>;

		using Vec2 = VboElementData<GLfloat, 2>;
		using Vec3 = VboElementData<GLfloat, 3>;
		using Vec4 = VboElementData<GLfloat, 4>;
		using Mat2 = VboElementData<GLfloat, 4>;
		using Mat3 = VboElementData<GLfloat, 9>;
		using Mat4 = VboElementData<GLfloat, 16>;

		using Ivec2 = VboElementData<GLint, 2>;
		using Ivec3 = VboElementData<GLint, 3>;
		using IVec4 = VboElementData<GLint, 4>;
		using Imat2 = VboElementData<GLint, 4>;
		using Imat3 = VboElementData<GLint, 9>;
		using Imat4 = VboElementData<GLint, 16>;

		using Uvec2 = VboElementData<GLuint, 2>;
		using Uvec3 = VboElementData<GLuint, 3>;
		using UVec4 = VboElementData<GLuint, 4>;
		using Umat2 = VboElementData<GLuint, 4>;
		using Umat3 = VboElementData<GLuint, 9>;
		using Umat4 = VboElementData<GLuint, 16>;

		using Dvec2 = VboElementData<GLdouble, 2>;
		using Dvec3 = VboElementData<GLdouble, 3>;
		using DVec4 = VboElementData<GLdouble, 4>;
		using Dmat2 = VboElementData<GLdouble, 4>;
		using Dmat3 = VboElementData<GLdouble, 9>;
		using Dmat4 = VboElementData<GLdouble, 16>;
	}

	class IVbo {
	public:
		IVbo(IVbo&) = delete;
		IVbo& operator=(IVbo&) = delete;
		IVbo(IVbo&& other) noexcept;
		IVbo& operator=(IVbo&& other) noexcept;
		virtual ~IVbo();

		[[nodiscard]] bool isValid() const;

		void bind() const;
		void unbind() const;

	protected:
		IVbo();

	private:
		GLuint _id;
	};

	inline IVbo::IVbo(IVbo&& other) noexcept :
		_id(other._id)
	{
		other._id = 0;
	}

	inline IVbo& IVbo::operator=(IVbo&& other) noexcept {
		_id = other._id;
		other._id = 0;

		return* this;
	}

	inline IVbo::~IVbo() {
		if(isValid())
			glDeleteBuffers(1, &_id);
	}

	inline bool IVbo::isValid() const {
		return _id != 0;
	}

	inline void IVbo::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, _id);
	}

	inline void IVbo::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	inline IVbo::IVbo() {
		glGenBuffers(1, &_id);
	}


	// TODO: Allow editing specific elements from the vbo and sending them to the gpu later on
	template<typename ... T>
	class Vbo : public IVbo {
	public:
		Vbo(const std::vector<GLuint>& indices, GLenum usage);

		void store(std::vector<std::tuple<T...>> data); // Store data

	private:
		template<GLuint Index, typename First, typename Second, typename ... Q>
		void bindIndices(const std::vector<GLuint>& indices);

		template<GLuint Index, typename Last>
		void bindIndices(const std::vector<GLuint>& indices);

		std::vector<std::tuple<T...>> _data;
		size_t _allocated = 0;
		GLenum _usage;
	};

	template <typename ... T>
	Vbo<T...>::Vbo(const std::vector<GLuint>& indices, GLenum usage) :
		_usage(usage)
	{
		assert(indices.size() == sizeof...(T));

		bind();
		bindIndices<0, T...>(indices);
		unbind();
	}

	template <typename ... T>
	void Vbo<T...>::store(std::vector<std::tuple<T...>> data) {
		if(data.size() > _allocated) {
			_allocated = data.size();
			_data = std::move(data);
			glBufferData(GL_ARRAY_BUFFER, _allocated * (sizeof(T) + ... + 0), &_data[0], _usage);
		}else {
			_data = std::move(data);
			glBufferSubData(GL_ARRAY_BUFFER, 0, _allocated * (sizeof(T) + ... + 0), &_data[0]);
		}
	}

	template <typename ... T>
	template <GLuint Index, typename First, typename Second, typename ... Q>
	void Vbo<T...>::bindIndices(const std::vector<GLuint>& indices) {
		glVertexAttribPointer(
			indices[Index],
			First::SIZE * sizeof(First::TYPE),
			First::getType(),
			GL_FALSE,
			0,
			nullptr
		);
		bindIndices<Index + 1, Second, Q...>(indices);
	}

	template <typename ... T>
	template <GLuint Index, typename Last>
	void Vbo<T...>::bindIndices(const std::vector<GLuint>& indices) {
		glVertexAttribPointer(
			indices[Index],
			Last::SIZE,
			Last::getType(),
			GL_FALSE,
			(sizeof(T) + ... + 0),
			nullptr
		);
	}

};