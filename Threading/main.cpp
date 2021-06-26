#include "concurrent/concurrent_queue.hpp"

#include <iostream>
#include "thread_pool.hpp"

void sleep_milliseconds(long long time)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(time));
}

auto get_tick_count()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

#define TEST 1

int main(void)
{
	long long pool_time = 0ll, thrd_time = 0ll;
	std::function<void(int)> f = [](int i)
	{
		for (int j = 0; j < 100; ++j)
		{
			std::cout << i << std::endl;
		}
	};

#if TEST
				
	{
		auto beg = 0ll;

		{
			timothy::thread_pool tp{ 20 };
			beg = get_tick_count();

			for (int i = 0; i < 2000; ++i)
			{
				tp.emplace(f, i);
			}
		}

		auto end = get_tick_count();
		pool_time = end - beg;
	}

#else

	{
		auto beg = get_tick_count();
		{
			std::vector<std::unique_ptr<std::thread>> v(2000);
			for (int m = 0; m < 100; ++m)
			{
				for (int i = 0; i < 20; ++i)
				{
					v[m * 20 + i] = std::make_unique<std::thread>(f, m * 20 + i);
				}
				int endi = m * 20 + 20;
				for (int i = endi - 20; i < endi; ++i)
				{
					v[i]->join();
				}
			}
		}
		auto end = get_tick_count();
		thrd_time = end - beg;
	}

#endif

	std::cout << "thread_pool: " << pool_time << std::endl;
	std::cout << "thread: " << thrd_time << std::endl;

	return 0;
}
