#pragma once
#include <memory>

namespace ecs {

	// Some very unsafe virtual functions, which are used by the ComponentStorage class
	class IMemoryPoolAllocator {
	friend class ComponentStorage;
	friend class Component;

	private:
		virtual void free(void* element) = 0;
		virtual void* insert(void* data) = 0;
		virtual std::unique_ptr<IMemoryPoolAllocator> copy() = 0;
	};

	// Just a basic memory pool allocator, might want to optimize this in the future
	template<typename T>
	class MemoryPoolAllocator : public IMemoryPoolAllocator {
	public:
		class MemoryPool {
		public:
			union Element {
			public:
				Element* getNext();
				void setNext(Element* next);

				T* getStorage();

			private:
				struct alignas(alignof(T)) StorageType {
					char a[sizeof(T)];
				};

				Element* _next;
				StorageType _data;
			};

			explicit MemoryPool(size_t size);

			void setNext(std::unique_ptr<MemoryPool> next) {
				_next = std::move(next);
			}

			Element* getStorage();

		private:
			std::unique_ptr<Element[]> _data;
			std::unique_ptr<MemoryPool> _next;
		};

		explicit MemoryPoolAllocator(size_t poolSize);
		MemoryPoolAllocator(MemoryPoolAllocator& other) = delete;
		MemoryPoolAllocator& operator=(MemoryPoolAllocator& other) = delete;
		MemoryPoolAllocator(MemoryPoolAllocator&&) = default;
		MemoryPoolAllocator& operator=(MemoryPoolAllocator&&) = default;
		~MemoryPoolAllocator() = default;

		template<typename ... Args>
		T* allocate(Args&& ... args);

		void free(T* t);

	private:
		size_t _poolSize;
		std::unique_ptr<MemoryPool> _currentPool;
		typename MemoryPool::Element* _freeSpaces;

		void* insert(void* data) override;
		void free(void* element) override {
			free(reinterpret_cast<T*>(element));
		};
		std::unique_ptr<IMemoryPoolAllocator> copy() override;
	};

	template <typename T>
	MemoryPoolAllocator<T>::MemoryPool::MemoryPool(size_t size) : _data(new Element[size]) {
		for (size_t i = 1; i < size; i++)
			_data[i - 1].setNext(&_data[i]);

		_data[size - 1].setNext(nullptr);
	}

	template <typename T>
	typename MemoryPoolAllocator<T>::MemoryPool::Element* MemoryPoolAllocator<T>::MemoryPool::getStorage() {
		return _data.get();
	}

	template <typename T>
	MemoryPoolAllocator<T>::MemoryPoolAllocator(size_t poolSize) :
		_poolSize(poolSize),
		_currentPool(std::make_unique<MemoryPool>(poolSize)),
		_freeSpaces(_currentPool->getStorage()) {}

	template <typename T>
	template <typename ... Args>
	T* MemoryPoolAllocator<T>::allocate(Args&& ... args) {
		if (_freeSpaces == nullptr) {
			// No free spaces available, create a new pool
			std::unique_ptr<MemoryPool> newPool = std::make_unique<MemoryPool>(_poolSize);
			newPool->setNext(std::move(_currentPool));
			_currentPool = std::move(newPool);
			_freeSpaces = _currentPool->getStorage();
		}

		auto* currentItem = _freeSpaces;
		_freeSpaces = currentItem->getNext();
		T* result = currentItem->getStorage();

		new (result) T(std::forward<Args>(args)...);

		return result;
	}

	template<typename T>
	inline void* MemoryPoolAllocator<T>::insert(void* data) {
		if constexpr (std::is_move_constructible_v<T>) {
			return allocate(std::move(*reinterpret_cast<T*>(data)));
		} else {
			return allocate(*reinterpret_cast<const T*>(data));
		}
	}

	template <typename T>
	void MemoryPoolAllocator<T>::free(T* t) {
		if (t == nullptr)
			return;

		t->T::~T();

		auto* e = reinterpret_cast<typename MemoryPool::Element*>(t);
		
		//TODO: sort the list maybe?
		e->setNext(_freeSpaces);
		_freeSpaces = e;
	}

	template<typename T>
	inline std::unique_ptr<IMemoryPoolAllocator> MemoryPoolAllocator<T>::copy() {
		return std::make_unique<MemoryPoolAllocator<T>>(_poolSize);
	}

	template <typename T>
	typename MemoryPoolAllocator<T>::MemoryPool::Element* MemoryPoolAllocator<T>::MemoryPool::Element::getNext() {
		return _next;
	}

	template <typename T>
	void MemoryPoolAllocator<T>::MemoryPool::Element::setNext(Element* next) {
		_next = next;
	}

	template <typename T>
	T* MemoryPoolAllocator<T>::MemoryPool::Element::getStorage() {
		return reinterpret_cast<T*>(_data.a);
	}
}