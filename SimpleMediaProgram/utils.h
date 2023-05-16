#pragma once

#include <algorithm>


template <typename T>
inline T clip(T value, T _min_value, T _max_value)
{
	return std::max(std::min(value, _max_value), _min_value);
}

template <typename T>
void remove_if_memory_exists(T* data)
{
	if (data)
		delete[] data;
	data = nullptr;
}

