#ifndef __BASIC_STRING_H__
#define __BASIC_STRING_H__

#include"Iterator.h"

#define DEFAULT_SIZE 15

template<typename CharT, typename Traits = std::char_traits<CharT>, typename Allocator = std::allocator<CharT>>
class BASIC_STRING
{
private:
	Allocator m_allocator; // default allocator
	CharT* m_data; // chuỗi
	size_t m_length; // số lượng kí tự chuỗi
	size_t m_capacity; // khoảng không gian dự trữ(use_memmory + not_use_memmory

	bool safeMalloc(size_t size)
	{
		if (size > m_capacity)
		{
			size_t k = (size - m_capacity) / DEFAULT_SIZE, newSize = m_capacity + (k + 1) * DEFAULT_SIZE;
			if (newSize + 1 > newSize)
			{
				CharT* oldData = m_data;
				m_data = NULL;
				m_data = m_allocator.allocate(newSize + 1);
				if (m_data == NULL)
				{
					m_data = oldData;
					return false;
				}
				safeFree(oldData, m_capacity);
				m_capacity = newSize;
			}
			else // size vượt quá max của size_t
				return false;
		}
		return true;
	}
	bool safeRealloc(size_t size)
	{
		if (size > m_capacity)
		{
			size_t k = (size - m_capacity) / DEFAULT_SIZE, newSize = m_capacity + (k + 1) * DEFAULT_SIZE;
			if (newSize + 1 > newSize)
			{
				CharT* oldData = m_data;
				m_data = NULL;
				m_data = m_allocator.allocate(newSize + 1);
				if (m_data == NULL)
				{
					m_data = oldData;
					return false;
				}
				Traits::copy(m_data, oldData, m_length + 1);
				safeFree(oldData, m_capacity);
				m_capacity = newSize;
			}
			else
				return false;
		}
		return true;
	}
	void safeFree(CharT* &other, size_t capacity)
	{
		if (other)
			m_allocator.deallocate(other, capacity + 1);
	}
	void safeFirstConstructor(BASIC_STRING<CharT> &other)
	{
		other.m_data = other.m_allocator.allocate(DEFAULT_SIZE + 1);
		other.m_allocator.construct(m_data, '\0');
		other.m_length = 0;
		other.m_capacity = DEFAULT_SIZE;
	}
	void safeSecondConstructor(const CharT* other, size_t count)
	{
		count = (count > Traits::length(other)) ? Traits::length(other) : count;
		if (safeMalloc(count))
		{
			Traits::copy(m_data, other, count);
			m_data[count] = '\0';
			m_length = count;
		}
	}
	void safeDestructor(BASIC_STRING<CharT> &other)
	{
		safeFree(other.m_data, other.m_capacity);
		other.m_data = NULL;
		other.m_length = other.m_capacity = 0;
	}
public:
	typedef typename CharT value_type;
	typedef typename Traits traits_type;
	typedef typename Allocator allocator_type;
	typedef typename Allocator::size_type size_type;
	typedef typename Allocator::difference_type difference_type;
	typedef typename Allocator::reference reference;
	typedef typename Allocator::const_reference const_reference;
	typedef typename Allocator::pointer pointer;
	typedef typename Allocator::const_pointer const_pointer;
	typedef typename RANDOM_ACCESS_ITERATOR<value_type> iterator;
	typedef typename const RANDOM_ACCESS_ITERATOR<value_type> const_iterator;
	typedef typename std::reverse_iterator<iterator> reverse_iterator;
	typedef typename std::reverse_iterator<const_iterator> const_reverse_iterator;
	static const size_type npos;

	BASIC_STRING()
	{
		safeFirstConstructor(*this);
	}
	explicit BASIC_STRING(const allocator_type &alloc) : m_allocator(alloc) 
	{
		safeFirstConstructor(*this);
	}
	BASIC_STRING(size_type count, value_type ch, const allocator_type &alloc = allocator_type()) : BASIC_STRING(alloc)
	{
		if (safeMalloc(count))
		{
			traits_type::assign(m_data, count, ch);
			m_data[count] = '\0';
			m_length = count;
		}
	}
	BASIC_STRING(const_pointer other, size_type count, const allocator_type &alloc = allocator_type()) : BASIC_STRING(alloc)
	{
		safeSecondConstructor(other, count);
	}
	BASIC_STRING(const_pointer other, const allocator_type &alloc = allocator_type()) : BASIC_STRING(other, traits_type::length(other), alloc) {}
	BASIC_STRING(const BASIC_STRING<value_type> &other, size_type index, size_type count = npos, const allocator_type &alloc = allocator_type()) 
	: BASIC_STRING(alloc)
	{
		try
		{
			if (index > other.m_length)
				throw std::out_of_range("Index is out of range :(");
			safeSecondConstructor(other.m_data + index, count);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	BASIC_STRING(const BASIC_STRING<value_type> &other, const allocator_type &alloc) : BASIC_STRING(other.m_data, alloc) {}
	BASIC_STRING(const BASIC_STRING<value_type> &other) : BASIC_STRING(other.m_data, other.m_allocator) {}
	template<typename InputIterator>
	BASIC_STRING(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type()) : BASIC_STRING(&(*first), last - first, alloc) {}
	BASIC_STRING(std::initializer_list<value_type> ilist, const allocator_type& alloc = allocator_type()) : BASIC_STRING(ilist.begin(), ilist.end(), alloc) {}
	~BASIC_STRING()
	{
		safeDestructor(*this);
	}

	BASIC_STRING<value_type>& operator=(const_reference ch)
	{
		return assign(1, ch);
	}
	BASIC_STRING<value_type>& operator=(const_pointer other)
	{
		return assign(other);
	}
	BASIC_STRING<value_type>& operator=(const BASIC_STRING<value_type> &other)
	{
		return assign(other);
	}
	BASIC_STRING<value_type>& operator=(std::initializer_list<value_type> ilist)
	{
		return assign(ilist);
	}

	BASIC_STRING<value_type>& assign(size_type count, const_reference ch)
	{
		if (safeMalloc(count))
		{
			m_data = traits_type::assign(m_data, count, ch);
			m_data[count] = '\0';
			m_length = count;
		}
		return *this;
	}
	BASIC_STRING<value_type>& assign(const_pointer other, size_type count)
	{
		safeSecondConstructor(other, count);
		return *this;
	}
	BASIC_STRING<value_type>& assign(const_pointer other)
	{
		return assign(other, traits_type::length(other));
	}
	BASIC_STRING<value_type>& assign(const BASIC_STRING<value_type> &other, size_type index, size_type count = npos)
	{
		try
		{
			if (index > other.m_length)
				throw std::out_of_range("Index is out of range :(");
			assign(other.m_data + index, count);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return *this;
	}
	BASIC_STRING<value_type>& assign(const BASIC_STRING<value_type> &other)
	{
		return assign(other.m_data);
	}
	template<typename InputIterator>
	BASIC_STRING<value_type>& assign(InputIterator first, InputIterator last)
	{
		return assign(&(*first), last - first);
	}
	BASIC_STRING<value_type>& assign(std::initializer_list<value_type> ilist)
	{
		return assign(ilist.begin(), ilist.end());
	}

	allocator_type get_allocator() const 
	{ 
		return BASIC_STRING<value_type, traits_type, allocator_type>::m_allocator; 
	}
	//----------------------------------------------- ELEMENT ACCESS --------------------------------------------------
	reference at(size_type pos)
	{
		try
		{
			if (pos >= m_length)
				throw std::out_of_range("Pos is out of range :(");
			return m_data[pos];
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	const_reference at(size_type pos) const
	{
		return at(pos);
	}

	reference operator[](size_type pos)
	{
		return m_data[pos];
	}
	const_reference operator[](size_type pos) const
	{
		return m_data[pos];
	}

	reference front()
	{
		return *m_data;
	}
	const_reference front() const
	{
		return *m_data;
	}

	reference back()
	{
		return m_data[m_length - 1];
	}
	const_reference back() const
	{
		return m_data[m_length - 1];
	}

	pointer data()
	{
		return m_data;
	}
	const_pointer data() const
	{
		return m_data;
	}
	const_pointer c_str() const
	{
		return m_data;
	}
	//------------------------------------------------ ITERATOR -------------------------------------------------------
	iterator begin()
	{
		return iterator(m_data);
	}
	const_iterator begin() const
	{
		return const_iterator(m_data);
	}
	const_iterator cbegin() const
	{
		return const_iterator(m_data);
	}
	reverse_iterator rbegin()
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(cend() - 1);
	}
	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(cend() - 1);
	}

	iterator end()
	{
		return iterator(m_data + m_length);
	}
	const_iterator end() const
	{
		return const_iterator(m_data + m_length);
	}
	const_iterator cend() const
	{
		return const_iterator(m_data + m_length);
	}
	reverse_iterator rend()
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(cbegin() - 1);
	}
	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(cbegin() - 1);
	}
	//------------------------------------------------ CAPACITY --------------------------------------------------------
	bool empty() const
	{
		return (begin() == end());
	}

	size_type size() const
	{
		return m_length;
	}
	size_type length() const
	{
		return m_length;
	}
	size_type max_size() const
	{
		return 4294967294;
	}

	void reserve(size_type size)
	{
		safeRealloc(size);
	}

	size_type capacity() const
	{
		return m_capacity;
	}

	void shrink_to_fit()
	{
		safeRealloc(m_length);
	}
	//------------------------------------------------ OPERATION --------------------------------------------------------
	void clear()
	{
		erase(begin(), end());
	}

	BASIC_STRING<value_type>& insert(size_type index, size_type count, const_reference ch)
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");
			
			if (safeRealloc(m_length + count))
			{
				size_type i;
				for (i = m_length + count; i >= index + count; i--)
					m_data[i] = m_data[i - count];
				for (i = index; i < index + count; i++)
					m_data[i] = ch;
				m_length += count;
			}
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return *this;
	}
	BASIC_STRING<value_type>& insert(size_type index, const_pointer other, size_type count)
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");

			if (safeRealloc(m_length + count))
			{
				size_type i;
				for (i = m_length + count; i >= index + count; i--)
					m_data[i] = m_data[i - count];
				for (i = index; i < index + count; i++)
					m_data[i] = other[i - index];
				m_length += count;
			}
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return *this;
	}
	BASIC_STRING<value_type>& insert(size_type index, const_pointer other)
	{
		return insert(index, other, traits_type::length(other));
	}
	BASIC_STRING<value_type>& insert(size_type index, const BASIC_STRING<value_type> &other, size_type index_other, size_type count = npos)
	{
		try
		{
			if (index_other > other.size())
				throw std::out_of_range("Index_other is out of range :(");
			return insert(index, other.m_data + index_other, count);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return *this;
	}
	BASIC_STRING<value_type>& insert(size_type index, const BASIC_STRING<value_type> &other)
	{
		return insert(index, other.m_data);
	}
	iterator insert(iterator pos, const_reference ch)
	{
		size_type distance = pos - begin();
		insert(distance, 1, ch);
		return begin() + distance;
	}
	iterator insert(iterator pos, size_type count, const_reference ch)
	{
		size_type distance = pos - begin();
		insert(distance, count, ch);
		return begin() + distance;
	}
	template<typename InputIterator>
	iterator insert(iterator pos, InputIterator first, InputIterator last)
	{
		size_type distance = pos - begin();
		insert(distance, &(*first), last - first);
		return begin() + distance;
	}
	iterator insert(iterator pos, std::initializer_list<value_type> ilist)
	{
		return insert(pos, ilist.begin(), ilist.end());
	}

	BASIC_STRING<value_type>& erase(size_type index = 0, size_type count = npos)
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");

			count = (count > m_length - index) ? m_length - index : count;
			for (size_type i = index; count + i <= m_length; i++)
				m_data[i] = m_data[count + i];
			m_length -= count;
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return *this;
	}
	iterator erase(iterator pos)
	{
		iterator it = begin();
		while (it != end())
		{
			if (it == pos)
			{
				erase(it - begin(), 1);
				break;
			}
			it++;
		}
		return it;
	}
	iterator erase(iterator first, iterator last)
	{
		erase(first - begin(), last - first);
		return first;
	}

	void push_back(const_reference ch)
	{
		return append(1, ch);
	}
	void pop_back()
	{
		erase(end() - 1);
	}

	BASIC_STRING<value_type>& append(size_type count, const_reference ch)
	{
		return insert(m_length, count, ch);
	}
	BASIC_STRING<value_type>& append(const_pointer other, size_type count)
	{
		return insert(m_length, other, count);
	}
	BASIC_STRING<value_type>& append(const_pointer other)
	{
		return append(other, traits_type::length(other));
	}
	BASIC_STRING<value_type>& append(const BASIC_STRING<value_type> &other, size_type count)
	{
		return append(other.m_data, count);
	}
	BASIC_STRING<value_type>& append(const BASIC_STRING<value_type> &other)
	{
		return append(other.m_data);
	}
	template<typename InputIterator>
	BASIC_STRING<value_type>& append(InputIterator first, InputIterator last)
	{
		return append(&(*first), last - first);
	}
	BASIC_STRING<value_type>& append(std::initializer_list<value_type> ilist)
	{
		return append(ilist.begin(), ilist.end());
	}

	BASIC_STRING<value_type>& operator+=(const_pointer other)
	{
		return append(other);
	}
	BASIC_STRING<value_type>& operator+=(const BASIC_STRING<value_type> &other)
	{
		return append(other);
	}
	BASIC_STRING<value_type>& operator+=(const_reference ch)
	{
		return append(1, ch);
	}
	BASIC_STRING<value_type>& operator+=(std::initializer_list<value_type> ilist)
	{
		return append(ilist);
	}

	int compare(size_type index, size_type count, const_pointer other, size_type count_other) const
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");

			count = (count > m_length - index) ? m_length - index : count;
			count_other = (count_other > traits_type::length(other)) ? traits_type::length(other) : count_other;
			int result = traits_type::compare(m_data + index, other, (count < count_other) ? count : count_other);

			if (result)
				return result;
			if (count < count_other)
				return -1;
			if (count > count_other)
				return 1;
			return 0;
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	int compare(size_type index, size_type count, const_pointer other) const
	{
		return compare(index, count, other, traits_type::length(other));
	}
	int compare(const_pointer other) const
	{
		return compare(0, m_length, other);
	}
	int compare(size_type index, size_type count, const BASIC_STRING<value_type> &other, size_type index_other, size_type count_other = npos) const
	{
		try
		{
			if (index_other > other.m_length)
				throw std::out_of_range("Index_other is out of range :(");
			return compare(index, count, other.m_data + index_other, count_other);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
	int compare(size_type index, size_type count, const BASIC_STRING<value_type> &other) const
	{
		return compare(index, count, other, 0, other.m_length);
	}
	int compare(const BASIC_STRING<value_type> &other) const
	{
		return compare(0, m_length, other);
	}

	BASIC_STRING<value_type>& replace(size_type index, size_type count, size_type count_other, const_reference ch)
	{
		erase(index, count);
		return insert(index, count_other, ch);
	}
	BASIC_STRING<value_type>& replace(size_type index, size_type count, const_pointer other, size_type count_other)
	{
		erase(index, count);
		return insert(index, other, count_other);
	}
	BASIC_STRING<value_type>& replace(size_type index, size_type count, const_pointer other)
	{
		return replace(index, count, other, traits_type::length(other));
	}
	BASIC_STRING<value_type>& replace(iterator first, iterator last, size_type count_other, const_reference ch)
	{
		erase(first, last);
		return insert(first - begin(), count_other, ch);
	}
	BASIC_STRING<value_type>& replace(iterator first, iterator last, const_pointer other, size_type count_other)
	{
		erase(first, last);
		return insert(first - begin(), other, count_other);
	}
	BASIC_STRING<value_type>& replace(iterator first, iterator last, const_pointer other)
	{
		return replace(first, last, other, traits_type::length(other));
	}
	BASIC_STRING<value_type>& replace(size_type index, size_type count, const BASIC_STRING<value_type> &other, size_type index_other, size_type count_other = npos)
	{
		try
		{
			if (index_other > other.m_length)
				throw std::out_of_range("Index_other is out of range :(");
			return replace(index, count, other.m_data + index_other, count_other);
		}
		catch (std::exception e)
		{
			std::cerr << e.what << std::endl;
		}
	}
	BASIC_STRING<value_type>& replace(iterator first, iterator last, const BASIC_STRING<value_type> &other)
	{
		return replace(first, last, other.m_data);
	}
	BASIC_STRING<value_type>& replace(size_type index, size_type count, const BASIC_STRING<value_type> &other)
	{
		return replace(index, count, other.m_data);
	}
	template<typename InputIterator >
	BASIC_STRING<value_type>& replace(iterator first, iterator last, InputIterator first_other, InputIterator last_other)
	{
		insert(erase(first, last), first_other, last_other);
		return *this;
	}
	BASIC_STRING<value_type>& replace(iterator first, iterator last, std::initializer_list<value_type> ilist)
	{
		return replace(first, last, ilist.begin(), ilist.end());
	}

	BASIC_STRING<value_type> substr(size_type index = 0, size_type count = npos) const
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");
			return BASIC_STRING(m_data + index, count, m_allocator);
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
	}

	size_type copy(pointer dest, size_type count, size_type index = 0) const
	{
		try
		{
			if (index > m_length)
				throw std::out_of_range("Index is out of range :(");
			count = (count > m_length - index) ? m_length - index : count;
			traits_type::copy(dest, m_data + index, count);
			return count;
		}
		catch (std::exception e)
		{
			std::cerr << e.what() << std::endl;
		}
		return 0;
	}

	void resize(size_type count, const_reference ch)
	{
		if (safeRealloc(count))
		{
			if (count > m_length)
				append(count - m_length, ch);
			else
			{
				m_data[count] = '\0';
				m_length = count;
			}
		}
	}
	void resize(size_type count)
	{
		resize(count, '\0');
	}

	void swap(BASIC_STRING<value_type> &other)
	{
		BASIC_STRING<value_type> temp(other);
		safeDestructor(other); safeFirstConstructor(other);
		other = *this;
		safeDestructor(*this); safeFirstConstructor(*this);
		*this = temp;
	}

	size_type find(value_type ch, size_type index = 0) const
	{
		while (index < m_length)
		{
			if (m_data[index] == ch)
				return index;
			index++;
		}
		return npos;
	}
	size_type find(const_pointer other, size_type index, size_type count) const
	{
		while (count <= m_length - index)
		{
			if (compare(index, count, other, count) == 0)
				return index;
			index++;
		}
		return npos;
	}
	size_type find(const_pointer other, size_type index = 0) const
	{
		return find(other, index, traits_type::length(other));
	}
	size_type find(const BASIC_STRING<value_type> &other, size_type index = 0) const
	{
		return find(other.m_data, index);
	}
	
	size_type rfind(value_type ch, size_type index = npos) const
	{
		index = (index >= m_length - 1) ? m_length - 1 : index;
		while (index >= 0)
		{
			if (m_data[index] == ch)
				return index;
			index--;
		}
		return npos;
	}
	size_type rfind(const_pointer other, size_type index, size_type count) const
	{
		index = (index >= m_length - 1) ? m_length - 1 : index;
		while (count <= index + 1)
		{
			if (compare(index - count + 1, count, other, count) == 0)
				return index - count + 1;
			index--;
		}
		return npos;
	}
	size_type rfind(const_pointer other, size_type index = npos) const
	{
		return rfind(other, index, traits_type::length(other));
	}
	size_type rfind(const BASIC_STRING<value_type> &other, size_type index = npos) const
	{
		return rfind(other.m_data, index);
	}

	size_type find_first_of(value_type ch, size_type index = 0) const
	{
		return find(ch, index);
	}
	size_type find_first_of(const_pointer other, size_type index, size_type count) const
	{
		while (index < m_length)
		{
			for (size_type i = 0; i < count; i++)
			{
				if (m_data[index] == other[i])
					return index;
			}
			index++;
		}
		return npos;
	}
	size_type find_first_of(const_pointer other, size_type index = 0) const
	{
		return find_first_of(other, index, traits_type::length(other));
	}
	size_type find_first_of(const BASIC_STRING<value_type> &other, size_type index = 0) const
	{
		return find_first_of(other.m_data, index);
	}

	size_type find_first_not_of(value_type ch, size_type index = 0) const
	{
		while (index < m_length)
		{
			if (m_data[index] != ch)
				return index;
			index++;
		}
		return npos;
	}
	size_type find_first_not_of(const_pointer other, size_type index, size_type count) const
	{
		bool flag;
		while (index < m_length)
		{
			flag = false;
			for (size_type i = 0; i < count; i++)
			{
				if (m_data[index] == other[i])
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
				return index;
			index++;
		}
		return npos;
	}
	size_type find_first_not_of(const_pointer other, size_type index = 0) const
	{
		return find_first_not_of(other, index, traits_type::length(other));
	}
	size_type find_first_not_of(const BASIC_STRING<value_type> &other, size_type index = 0) const
	{
		return find_first_not_of(other.m_data, index);
	}

	size_type find_last_of(value_type ch, size_type index = npos) const
	{
		return rfind(ch, index);
	}
	size_type find_last_of(const_pointer other, size_type index, size_type count) const
	{
		index = (index >= m_length - 1) ? m_length - 1 : index;
		while (index >= 0)
		{
			for (size_type i = 0; i < count; i++)
			{
				if (m_data[index] == other[i])
					return index;
			}
			index--;
		}
		return npos;
	}
	size_type find_last_of(const_pointer other, size_type index = npos) const
	{
		return find_last_of(other, index, traits_type::length(other));
	}
	size_type find_last_of(const BASIC_STRING<value_type> &other, size_type index = npos) const
	{
		return find_last_of(other.m_data, index);
	}

	size_type find_last_not_of(value_type ch, size_type index = pos) const
	{
		index = (index >= m_length - 1) ? m_length - 1 : index;
		while (index >= 0)
		{
			if (m_data[index] != ch)
				return index;
			index--;
		}
		return npos;
	}
	size_type find_last_not_of(const_pointer other, size_type index, size_type count) const
	{
		index = (index >= m_length - 1) ? m_length - 1 : index;
		bool flag;
		while (index >= 0)
		{
			flag = false;
			for (size_type i = 0; i < count; i++)
			{
				if (m_data[index] == other[i])
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
				return index;
			index--;
		}
		return npos;
	}
	size_type find_last_not_of(const_pointer other, size_type index = 0) const
	{
		return find_last_not_of(other, index, traits_type::length(other));
	}
	size_type find_last_not_of(const BASIC_STRING<value_type> &other, size_type index = 0) const
	{
		return find_last_not_of(other.m_data, index);
	}
};
template<typename CharT, typename Traits, typename Allocator>
const size_t BASIC_STRING<CharT, Traits, Allocator>::npos = -1;
//------------------------------------------------ NONE MEMBER FUNCTIONS --------------------------------------------------------
template<typename CharT, typename Traits, typename Allocator>
BASIC_STRING<CharT, Traits, Allocator> operator+(const CharT &left, const BASIC_STRING<CharT, Traits, Allocator> &right)
{
	BASIC_STRING<CharT, Traits, Allocator> result(1, left, right.get_allocator());
	return result.append(right);
}
template<typename CharT, typename Traits, typename Allocator>
BASIC_STRING<CharT, Traits, Allocator> operator+(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT &right)
{
	BASIC_STRING<CharT, Traits, Allocator> result(left);
	return result.append(1, right);
}
template<typename CharT, typename Traits, typename Allocator>
BASIC_STRING<CharT, Traits, Allocator> operator+(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right)
{
	BASIC_STRING<CharT, Traits, Allocator> result(left, right.get_allocator());
	return result.append(right);
}
template<typename CharT, typename Traits, typename Allocator>
BASIC_STRING<CharT, Traits, Allocator> operator+(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right)
{
	BASIC_STRING<CharT, Traits, Allocator> result(left);
	return result.append(right);
}
template<typename CharT, typename Traits, typename Allocator>
BASIC_STRING<CharT, Traits, Allocator> operator+(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right)
{
	BASIC_STRING<CharT, Traits, Allocator> result(left);
	return result.append(right);
}

template<typename CharT, typename Traits, typename Allocator>
bool operator==(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (right.compare(left) == 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator==(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return (left.compare(right) == 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator!=(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left == right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator!=(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return !(left == right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (right.compare(left) > 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return (left.compare(right) < 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<=(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left > right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<=(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return !(left > right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (right.compare(left) < 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return (left.compare(right) > 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>=(const CharT* left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left < right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>=(const BASIC_STRING<CharT, Traits, Allocator> &left, const CharT* right) { return !(left < right); }

template<typename CharT, typename Traits, typename Allocator>
bool operator==(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (left.compare(right) == 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator!=(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left == right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (left.compare(right) < 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator<=(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left > right); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return (left.compare(right) > 0); }
template<typename CharT, typename Traits, typename Allocator>
bool operator>=(const BASIC_STRING<CharT, Traits, Allocator> &left, const BASIC_STRING<CharT, Traits, Allocator> &right) { return !(left < right); }

template<typename CharT, typename Traits, typename Allocator>
void swap(BASIC_STRING<CharT, Traits, Allocator> &left, BASIC_STRING<CharT, Traits, Allocator> &right)
{
	left.swap(right);
}

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits> &inDev, BASIC_STRING<CharT, Traits, Allocator> &object);
template<>
std::basic_istream<char>& operator>>(std::basic_istream<char> &inDev, BASIC_STRING<char> &object)
{
	object.clear();
	char temp;
	while (true)
	{
		temp = getchar();
		if (temp == ' ' || temp == '\n')
			break;
		object += temp;
	}
	return inDev;
}
template<>
std::basic_istream<wchar_t>& operator>>(std::basic_istream<wchar_t> &inDev, BASIC_STRING<wchar_t> &object)
{
	object.clear();
	wchar_t temp;
	while (true)
	{
		temp = getwchar();
		if (temp == ' ' || temp == '\n')
			break;
		object += temp;
	}
	return inDev;
}
template<typename CharT, typename Traits, typename Allocator>
std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits> &outDev, const BASIC_STRING<CharT, Traits, Allocator> &object)
{
	outDev << object.data();
	return outDev;
}

template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits> &inDev, BASIC_STRING<CharT, Traits, Allocator> &object, CharT delim);
template<typename CharT, typename Traits, typename Allocator>
std::basic_istream<CharT, Traits>& getline(std::basic_istream<CharT, Traits> &inDev, BASIC_STRING<CharT, Traits, Allocator> &object)
{
	return getline(inDev, object, inDev.widen('\n'));
}
template<>
std::basic_istream<char>& getline(std::basic_istream<char> &inDev, BASIC_STRING<char> &object, char delim)
{
	object.clear();
	char temp;
	while ((temp = getchar()) != delim)
		object += temp;
	return inDev;
}
template<>
std::basic_istream<wchar_t>& getline(std::basic_istream<wchar_t> &inDev, BASIC_STRING<wchar_t> &object, wchar_t delim)
{
	object.clear();
	wchar_t temp;
	while ((temp = getwchar()) != delim)
		object += temp;
	return inDev;
}

using STRING = BASIC_STRING<char>;
using WSTRING = BASIC_STRING<wchar_t>;

#endif