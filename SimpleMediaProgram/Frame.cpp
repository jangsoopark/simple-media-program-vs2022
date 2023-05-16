#include "Frame.h"

#include <utility>

#include "utils.h"

Frame::Frame(size_t height, size_t width)
	: data(new uint8_t [height * width * 3])
	, height(height)
	, width(width)
{
}

Frame::Frame(uint8_t* data, size_t height, size_t width)
	: data(std::move(data))
	, height(height)
	, width(width)
{
}

Frame::Frame(Frame&& frame)
	: data(std::move(frame.data))
	, height(frame.height)
	, width(frame.width)
{
}

Frame::~Frame()
{
	remove_if_memory_exists(this->data);
}

Frame& Frame::operator=(uint8_t* data)
{
	remove_if_memory_exists(this->data);
	this->data = data;
	return *this;
}

void Frame::setValue(uint8_t v, int i, int j)
{
	this->data[i * this->width + j * 3] = v;
}

uint8_t Frame::getValue(int i, int j)
{
	return this->data[i * this->width + j * 3];
}

uint8_t* Frame::getData()
{
	return this->data;
}

size_t Frame::getHeight()
{
	return this->height;
}

size_t Frame::getWidth()
{
	return this->width;
}
