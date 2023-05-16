#pragma once

#include <fstream>
#include <memory>
#include <string>

#include "Frame.h"


class VideoLoader
{
public:
	VideoLoader();
	VideoLoader(std::string path, size_t height, size_t width);
	~VideoLoader();

	bool open(std::string path, size_t height, size_t width);
	Frame& read(bool& success, Frame& frmae);
	
private:
	inline void yuv420_to_rgb(uint8_t& r, uint8_t& g, uint8_t& b, uint8_t y, uint8_t cb, uint8_t cr);

private:
	std::string path;
	size_t height;
	size_t width;

	size_t size;
	size_t stride;
	
	std::ifstream fin;
	uint8_t* buf;

};

