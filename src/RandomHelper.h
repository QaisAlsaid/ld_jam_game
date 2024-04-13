#ifndef RANDOM_HELPER_H
#define RANDOM_HELPER_H
#include <random>

class Random
{
public:
	static void init()
	{
		s_random_engine.seed(std::random_device()());
	}

	static float Float()
	{
		return (float)s_distribution(s_random_engine) / (float)std::numeric_limits<uint32_t>::max();
	}

private:
	static std::mt19937 s_random_engine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
};

#endif //RANDOM_HELPER_H
