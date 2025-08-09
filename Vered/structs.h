#pragma once

namespace vered
{
	struct buffer16
	{
	public:
		buffer16(short* buffer, int samples, int sample_rate) : buffer(buffer), samples(samples), sample_rate(sample_rate)
		{
		}

		short* buffer;
		int samples;
		int sample_rate;
	};

	struct bufferDouble
	{
		bufferDouble(double* buffer, int samples, int sample_rate) : buffer(buffer), samples(samples), sample_rate(sample_rate)
		{

		}

		double* buffer;
		int samples;
		int sample_rate;
	};
}