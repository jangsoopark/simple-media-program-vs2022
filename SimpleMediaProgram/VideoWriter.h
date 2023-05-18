#pragma once

#include <fstream>
#include <string>
#include "Frame.h"


class VideoWriter
{

public:
	VideoWriter();
	VideoWriter(std::string path, size_t height, size_t width);
	~VideoWriter();

	bool open(std::string path, size_t height, size_t width);
	void write(std::unique_ptr<Frame>& frame);

private:
	

private:
	std::string path;
	size_t height;
	size_t width;
	
	size_t size;
	size_t stride;

	std::ofstream fout;

};

