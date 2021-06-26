#pragma once

#ifndef THREAD_POOL_HPP

#define THREAD_POOL_HPP

#include <queue>
#include <thread>
#include <functional>
#include <memory>
#include <vector>
#include <condition_variable>
#include <atomic>
#include <timothy.h>

TIMOTHY_NAMESPACE_BEGIN

class thread_pool
{
private:
	using container_type = std::vector<std::unique_ptr<std::thread>>;

public:
	using size_type = typename container_type::size_type;

	thread_pool(size_type thread_size) : _thrds{ thread_size }
	{
		for (auto&& ptr : _thrds)
		{
			ptr = std::make_unique<std::thread>(&thread_pool::run, this);
		}
	}

	template <typename _Ret, typename... _Args>
	void emplace(std::function<_Ret(_Args...)> func, _Args... args)
	{
		std::unique_lock<std::mutex> lg{ _mtx };
		_q.emplace([func, args...]{ func(args...); });
		_cv.notify_one();
	}

	~thread_pool()
	{
		_end = true;
		_cv.notify_all();
		for (auto&& ptr : _thrds)
		{
			ptr->join();
		}
	}

private:
	void run()
	{
		while (true)
		{
			std::function<void()> fop;
			{
				std::unique_lock<std::mutex> lg{ _mtx };
				while (!_end && _q.empty())
				{
					_cv.wait(lg);
				}
				if (_q.empty()) break;
				fop = std::move(_q.front());
				_q.pop();
			}
			fop();
		}
	}

private:
	container_type _thrds;
	std::mutex _mtx;
	std::queue<std::function<void()>> _q;
	std::condition_variable _cv;
	std::atomic_bool _detach = false;
	std::atomic_bool _end = false;
};

TIMOTHY_NAMESPACE_END

#endif // !THREAD_POOL_HPP
