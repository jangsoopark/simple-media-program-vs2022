#include "VideoLoader.h"
#include "VideoWriter.h"
#include "config.h"

#include <iostream>
#include <fstream>


void down_sample(Frame& src, Frame& dst)
{
	int sh = (int)(src.getHeight() / dst.getHeight());
	int sw = (int)(src.getWidth() / dst.getWidth());

	auto src_data = src.getData();
	auto dst_data = dst.getData();

	int idx = 0;
	for (int i = 0; i < src.getHeight(); i += sh)
	{
		for (int j = 0; j < src.getWidth(); j += sw)
		{
			dst_data[idx++] = src_data[i * src.getWidth() * 3 + j * 3];
			dst_data[idx++] = src_data[i * src.getWidth() * 3 + j * 3 + 1];
			dst_data[idx++] = src_data[i * src.getWidth() * 3 + j * 3 + 2];
		}
	}
}


int main()
{
	Config config(
		"C:\\Users\\jangsoopark\\Desktop\\asdf\\ta\\2023\\oop\\project\\RaceHorses_416x240_30\\RaceHorses_416x240_30.yuv",
		"C:\\Users\\jangsoopark\\Desktop\\asdf\\ta\\2023\\oop\\project\\RaceHorses_416x240_30\\RaceHorses_208x120_30.rgb",
		240, 416, 3);

	VideoLoader video_loader(config.input_path, config.height, config.width);
	VideoWriter video_writer(config.output_path, (config.height >> 1), (config.width >> 1));

	Frame frame(config.height, config.width);
	Frame resized(config.height >> 1, config.width >> 1);

	bool ret = false;

	int i = 0;
	while (true)
	{
		video_loader.read(ret, frame);

		if (!ret)
			break;

		down_sample(frame, resized);
		video_writer.write(resized);
		i++;
	}

	std::cout << i << std::endl;

	return 0;
}