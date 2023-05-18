
#include <iostream>
#include <fstream>

#include <memory>
#include <thread>
#include <chrono>
#include <queue>

#include "VideoLoader.h"
#include "VideoWriter.h"
#include "config.h"


void down_sample(std::unique_ptr<Frame>& src, std::unique_ptr<Frame>& dst)
{
	int sh = (int)(src.get()->getHeight() / dst.get()->getHeight());
	int sw = (int)(src.get()->getWidth() / dst.get()->getWidth());

	auto src_data = src.get()->getData();
	auto dst_data = dst.get()->getData();

	int idx = 0;
	for (int i = 0; i < src.get()->getHeight(); i += sh)
	{
		for (int j = 0; j < src.get()->getWidth(); j += sw)
		{
			dst_data[idx++] = src_data[i * src.get()->getWidth() * 3 + j * 3];
			dst_data[idx++] = src_data[i * src.get()->getWidth() * 3 + j * 3 + 1];
			dst_data[idx++] = src_data[i * src.get()->getWidth() * 3 + j * 3 + 2];
		}
	}
}


int main()
{
	Config config(
		"C:\\Users\\jangsoopark\\Desktop\\asdf\\ta\\2023\\oop\\project\\RaceHorses_416x240_30\\RaceHorses_416x240_30.yuv",
		"C:\\Users\\jangsoopark\\Desktop\\asdf\\ta\\2023\\oop\\project\\RaceHorses_416x240_30\\RaceHorses_208x120_30.rgb",
		240, 416, 3);

	// Global memory to communicate between the main process and each child threads
	std::queue<std::unique_ptr<Frame> > input_video_queue;
	std::queue<std::unique_ptr<Frame> > output_video_queue;
	bool is_running = true;

	// thread to open a video file and then read the frame
	std::thread video_read_thread([&input_video_queue, &is_running](std::string path, size_t height, size_t width) {

		// Open the input video 
		VideoLoader video_loader(path, height, width);
		while (true)
		{
			// make a memory to store the video frame
			std::unique_ptr<Frame> frame = std::make_unique<Frame>(height, width);
			
			// read the video frame from file
			bool ret = video_loader.read(frame);
			if (!ret)
				break;
			
			input_video_queue.push(std::move(frame));
			std::cout << "read frame" << std::endl;
		}
		
		// send a message to exit 
		is_running = false;
		}, config.input_path, config.height, config.width);

	// thread to downsize the input video by half
	std::thread down_sample_x2_thread([&input_video_queue, &output_video_queue, &is_running]() {
		while (true)
		{
			// when the thread receive exit message and 
			// doesn't have anything to process, terimnate to loop
			if (input_video_queue.empty() && !is_running)
				break;

			if (input_video_queue.empty())
				continue;

			std::unique_ptr<Frame> original_frame_ptr = std::move(input_video_queue.front());
			input_video_queue.pop();
			
			std::unique_ptr<Frame> resized_frame_ptr = std::make_unique<Frame>(
				original_frame_ptr.get()->getHeight() >> 1, original_frame_ptr.get()->getWidth() >> 1);

			// down size
			down_sample(original_frame_ptr, resized_frame_ptr);

			output_video_queue.push(std::move(resized_frame_ptr));
			std::cout << "down sample X2" << std::endl;
		}
		});

	// thread to write downsized video into the file
	std::thread video_write_thread([&output_video_queue, &is_running](std::string path, size_t height, size_t width) {
		
		// Create a file to write the output video 
		VideoWriter video_writer(path, height, width);
		while (true)
		{
			// when the thread receive exit message and 
			// doesn't have anything to process, terimnate to loop
			if (output_video_queue.empty() && !is_running)
				break;

			if (output_video_queue.empty())
				continue;
			auto _ptr = std::move(output_video_queue.front());
			output_video_queue.pop();

			// write video frame
			video_writer.write(_ptr);

		}
		std::cout << "write frame" << std::endl;
		}, config.output_path, config.height >> 1, config.width >> 1);

	// join
	video_write_thread.join();
	down_sample_x2_thread.join();
	video_read_thread.join();
	
	return 0;
}