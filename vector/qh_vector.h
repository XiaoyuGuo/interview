#ifndef QIHOO_VECTOR_H_
#define QIHOO_VECTOR_H_

#include <assert.h>

namespace qh
{
	template<class T>
	class vector {
	public:
		//ctor
		vector()
			: data_(NULL), size_(0), capacity_(0)
		{
		}

		//Initialize T[n] with value
		explicit vector(size_t n, const T& value = T())
		{
			data_ = new T[n];
			capacity_ = n;
			size_ = n;

			for (size_t i = 0; i < n; i++)
			{
				data_[i] = value;
			}
		}

		vector<T>& operator=(const vector<T>& rhs)
		{
			// Avoid self
			if (this != &rhs)
			{
				if (data_) {
					delete[] data_;
				}

				size_ = rhs.size();
				capacity_ = size_;
				data_ = new T[capacity_];
				for (size_t i = 0; i < size_; i++)
				{
					// Not deep copy
					data_[i] = rhs.data_[i];
				}
			}
		}

		//dtor
		~vector()
		{
			if (data_)
			{
				delete[] data_;
			}
		}

		//get
		size_t size() const
		{
			return size_;
		}

		// set & get
		T& operator[](size_t index);

		// set
		void push_back(const T& element);
		void pop_back();
		void resize(size_t size);
		void reserve(size_t capacity);
		void clear();
		bool empty();

	private:
		T*      data_;
		size_t  size_;
		size_t capacity_;
	};

	template<class T>
	T& vector<T>::operator[](size_t index)
	{
		assert(index < size_);
		assert(index >= 0);
		return data_[index];
	}

	template<class T>
	void vector<T>::push_back(const T& element) {
		if (size_ == capacity_)
		{
			reserve(size_ * 2 + 1);
		}
		data_[size_] = element;
		size_++;
	}

	template<class T>
	void vector<T>::pop_back()
	{
		assert(size_ > 0);
		size_--;
	}

	template<class T>
	void vector<T>::reserve(size_t capacity)
	{
		if (capacity <= size_) return;

		T* tmp_data = data_;
		data_ = new T[capacity];

		for (size_t i = 0; i < size_; ++i)
		{
			data_[i] = tmp_data[i];
		}

		capacity_ = capacity;

		delete[] tmp_data;
	}

	template<class T>
	void vector<T>::resize(size_t size)
	{
		if (size > size_)
		{
			// Plus 1 avoid size == 0
			reserve(2 * size + 1);
		}
		else
		{
			size_ = size;
		}

	}

	template<class T>
	void vector<T>::clear()
	{
		size_ = 0;
	}

	template<class T>
	bool vector<T>::empty()
	{
		return size_ == 0;
	}
}
#endif


