#pragma once

#include <string>

struct Config
{
	Config(std::string&& input_path, std::string&& output_path, size_t height, size_t width, uint8_t channels)
		: input_path(std::move(input_path)), output_path(std::move(output_path))
		, height(height), width(width), channels(channels)
	{}

	std::string input_path;
	std::string output_path;
	size_t height;
	size_t width;
	uint8_t channels;
};