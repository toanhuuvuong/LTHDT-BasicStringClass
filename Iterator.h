#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include<iostream>

// OUTPUT_ITERATOR
template<typename T>
class OUTPUT_ITERATOR : public std::iterator<std::output_iterator_tag, T, std::ptrdiff_t, T*, T&>
{
private:
	pointer m_ptr;
public:
	OUTPUT_ITERATOR(pointer other) : m_ptr(other) {}
	OUTPUT_ITERATOR(const OUTPUT_ITERATOR<value_type> &other) : OUTPUT_ITERATOR(other.m_ptr) {}

	reference operator*() { return *m_ptr; }
	OUTPUT_ITERATOR<value_type>& operator++() { m_ptr++; return *this; }
	OUTPUT_ITERATOR<value_type> operator++(int) { pointer temp = m_ptr; m_ptr++;  return OUTPUT_ITERATOR(temp); }
};
// INPUT_ITERATOR
template<typename T>
class INPUT_ITERATOR : public std::iterator<std::input_iterator_tag, T, std::ptrdiff_t, T*, T&>
{
protected:
	pointer m_ptr;
public:
	INPUT_ITERATOR(pointer other) : m_ptr(other) {}
	INPUT_ITERATOR(const INPUT_ITERATOR<value_type>  &other) : INPUT_ITERATOR(other.m_ptr) {}

	reference operator*() { return *m_ptr; }
	pointer operator->() { return &*m_ptr; }
	INPUT_ITERATOR<value_type> & operator++() { m_ptr++; return *this; }
	INPUT_ITERATOR<value_type>  operator++(int) { pointer temp = m_ptr; m_ptr++;  return INPUT_ITERATOR(temp); }

	bool operator==(const INPUT_ITERATOR& other) const { return (m_ptr == other.m_ptr); }
	bool operator!=(const INPUT_ITERATOR& other) const { return (m_ptr != other.m_ptr); }
};
// FORWARD_ITERATOR
template<typename T>
class FORWARD_ITERATOR : public INPUT_ITERATOR<T>
{
public:
	FORWARD_ITERATOR() { m_ptr = NULL; }
	FORWARD_ITERATOR(pointer other) : INPUT_ITERATOR(other) {}
	FORWARD_ITERATOR(const FORWARD_ITERATOR<value_type>  &other) : INPUT_ITERATOR(other) {}

	FORWARD_ITERATOR<value_type>& operator++() { m_ptr++; return *this; }
	FORWARD_ITERATOR<value_type> operator++(int) { pointer temp = m_ptr; m_ptr++;  return FORWARD_ITERATOR(temp); }
};
// BIDIRECTIONAL_ITERATOR
template<typename T>
class BIDIRECTIONAL_ITERATOR : public FORWARD_ITERATOR<T>
{
public:
	BIDIRECTIONAL_ITERATOR() { m_ptr = NULL; }
	BIDIRECTIONAL_ITERATOR(pointer other) : FORWARD_ITERATOR(other) {}
	BIDIRECTIONAL_ITERATOR(const BIDIRECTIONAL_ITERATOR<value_type>  &other) : FORWARD_ITERATOR(other) {}

	BIDIRECTIONAL_ITERATOR<value_type>& operator++() { m_ptr++; return *this; }
	BIDIRECTIONAL_ITERATOR<value_type> operator++(int) { pointer temp = m_ptr; m_ptr++;  return BIDIRECTIONAL_ITERATOR(temp); }
	BIDIRECTIONAL_ITERATOR<value_type>& operator--() { m_ptr--; return *this; }
	BIDIRECTIONAL_ITERATOR<value_type> operator--(int) { pointer temp = m_ptr; m_ptr--; return BIDIRECTIONAL_ITERATOR(temp); }
};
// RANDOM_ACCESS_ITERATOR
template<typename T>
class RANDOM_ACCESS_ITERATOR : public BIDIRECTIONAL_ITERATOR<T>
{
public:
	RANDOM_ACCESS_ITERATOR() { m_ptr = NULL; }
	RANDOM_ACCESS_ITERATOR(pointer other) : BIDIRECTIONAL_ITERATOR(other) {}
	RANDOM_ACCESS_ITERATOR(const RANDOM_ACCESS_ITERATOR<value_type>  &other) : BIDIRECTIONAL_ITERATOR(other) {}

	RANDOM_ACCESS_ITERATOR<value_type>& operator++() { m_ptr++; return *this; }
	RANDOM_ACCESS_ITERATOR<value_type> operator++(int) { pointer temp = m_ptr; m_ptr++;  return RANDOM_ACCESS_ITERATOR(temp); }
	RANDOM_ACCESS_ITERATOR<value_type>& operator--() { m_ptr--; return *this; }
	RANDOM_ACCESS_ITERATOR<value_type> operator--(int) { pointer temp = m_ptr; m_ptr--; return RANDOM_ACCESS_ITERATOR(temp); }
	RANDOM_ACCESS_ITERATOR<value_type>& operator+=(const distance_type &n) { m_ptr += n; return *this; }
	RANDOM_ACCESS_ITERATOR<value_type>& operator-=(const distance_type &n) { m_ptr -= n; return *this; }
	RANDOM_ACCESS_ITERATOR<value_type> operator+(const distance_type &n) { return RANDOM_ACCESS_ITERATOR(m_ptr + n); }
	friend RANDOM_ACCESS_ITERATOR<value_type>  operator+(const distance_type &n,
		const RANDOM_ACCESS_ITERATOR<value_type> &object) {
		return RANDOM_ACCESS_ITERATOR(object.m_ptr + n);
	}
	RANDOM_ACCESS_ITERATOR<value_type> operator-(const distance_type &n) { return RANDOM_ACCESS_ITERATOR(m_ptr - n); }
	distance_type operator-(const RANDOM_ACCESS_ITERATOR<value_type>  &other) { return (m_ptr - other.m_ptr); }
	value_type& operator[](const distance_type &n) { return *(m_ptr + n); }

	bool operator<(const RANDOM_ACCESS_ITERATOR& other) const { return (m_ptr < other.m_ptr); }
	bool operator>(const RANDOM_ACCESS_ITERATOR& other) const { return (m_ptr > other.m_ptr); }
	bool operator<=(const RANDOM_ACCESS_ITERATOR& other) const { return (m_ptr <= other.m_ptr); }
	bool operator>=(const RANDOM_ACCESS_ITERATOR& other) const { return (m_ptr >= other.m_ptr); }
};
//--------------------------------------------------------------------------------------------------------------------------------------------

#endif
