#include "VideoLoader.h"
#include <algorithm>

#include <utility>

#include "utils.h"
#include <iostream>

VideoLoader::VideoLoader()
	: height(0), width(0), size(0), stride(0), buf(nullptr)
{
}

VideoLoader::VideoLoader(std::string path, size_t height, size_t width)
	: path(std::move(path)), height(height), width(width), size(0), stride(0), buf(nullptr)
{
	this->size = this->height * this->width;
	this->stride = this->size + (this->size >> 1);
	this->open(this->path, this->height, this->width);
}

VideoLoader::~VideoLoader()
{
	remove_if_memory_exists(this->buf);
	this->fin.close();
}

bool VideoLoader::open(std::string path, size_t height, size_t width)
{
	fin.open(path, std::ios::binary);
	if (fin.fail())
		return false;

	this->buf = new uint8_t[this->stride];
	
	return true;
}

bool VideoLoader::read( std::unique_ptr<Frame>& frame)
{
	this->fin.read((char*)(this->buf), this->stride);
	if (this->fin.eof() && !this->fin.gcount())
		return false;
	
	
	auto y = this->buf;
	auto cb = this->buf + this->size;
	auto cr = this->buf + this->size + (this->size >> 2);
	
	uint8_t r = 0, g = 0, b = 0;
	size_t image_idx = 0;

	auto _data = frame.get()->getData();
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			auto idx = i* width + j;
			auto color_idx = (i >> 1) * (width >> 1) + (j >> 1);
			
			this->yuv420_to_rgb(r, g, b, y[idx], cb[color_idx], cr[color_idx]);
			
			_data[image_idx++] = r;
			_data[image_idx++] = g;
			_data[image_idx++] = b;
		}
	}
	
	return true;
}

inline void VideoLoader::yuv420_to_rgb(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t y, uint8_t cb, uint8_t cr)
{
	int16_t _cb = (int16_t)cb - 128;
	int16_t _cr = (int16_t)cr - 128;
	
	r = (uint8_t)clip((int16_t)(y + 1.402 * _cr), (int16_t)0, (int16_t)255);
	g = (uint8_t)clip((int16_t)(y - 0.344 * _cb - 0.714 * _cr), (int16_t)0, (int16_t)255);
	b = (uint8_t)clip((int16_t)(y + 1.772 * _cb), (int16_t)0, (int16_t)255);
}

