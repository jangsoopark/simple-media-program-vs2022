#pragma once

#include <memory>

class Frame
{
public:
	Frame(size_t height, size_t width);
	Frame(uint8_t* data, size_t height, size_t width);
	Frame(Frame&& frame);
	~Frame();

	Frame& operator = (uint8_t* data);

	void setValue(uint8_t v, int i, int j);

	uint8_t getValue(int i, int j);
	uint8_t* getData();

	size_t getHeight();
	size_t getWidth();

private:
	
	uint8_t* data;
	size_t height;
	size_t width;
};

