#include "VideoWriter.h"

#include <fstream>

VideoWriter::VideoWriter()
	: height(0), width(0), size(0), stride(0)
{
}

VideoWriter::VideoWriter(std::string path, size_t height, size_t width)
	: path(std::move(path)), height(height), width(width)
	
{
	this->size = this->height * this->width;
	this->stride = this->size * 3;
	this->open(this->path, this->height, this->width);
}

VideoWriter::~VideoWriter()
{
	this->fout.close();
}

bool VideoWriter::open(std::string path, size_t height, size_t width)
{
	fout.open(path, std::ios::binary);
	if (fout.fail())
		return false;
	
	return true;
}

void VideoWriter::write(std::unique_ptr<Frame>& frame)
{
	auto data = frame.get();
	this->fout.write((char*)(data->getData()), this->stride);
}

