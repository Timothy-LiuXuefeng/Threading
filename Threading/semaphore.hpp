#pragma once

#ifndef SEMAPHORE_HPP

#define SEMAPHORE_HPP

#include <timothy.h>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <cstddef>
#include <exception>

TIMOTHY_NAMESPACE_BEGIN

class semaphore
{
public:

	class semaphore_overflow : public std::exception
	{
	public:
		[[nodiscard]] virtual const char* what() const override
		{
			return "Semaphore overflow!";
		}
	};

	using size_type = std::size_t;

	explicit semaphore(size_type max_count, size_type initial_count = 0) : _max_cnt(max_count), _cnt(initial_count) {}
	semaphore(const semaphore&) = delete;
	
	void wait_one()
	{
		std::unique_lock<std::mutex> lg{ _mtx };
		_cv.wait(lg, [this] { return _cnt > 0; });
		--_cnt;
	}
	
	[[nodiscard]] bool try_wait_one()
	{
		std::unique_lock<std::mutex> lg{ _mtx };
		if (_cnt > 0)
		{
			--_cnt;
			return true;
		}
		else return false;
	}

	void release_one()
	{
		std::unique_lock<std::mutex> lg{ _mtx };
		if (_cnt < _max_cnt)
		{
			++_cnt;
			_cv.notify_one();
		}
		else
		{
			throw semaphore_overflow{};
		}
	}

	[[nodiscard]] size_type max_count() const { return _max_cnt; }

	[[nodiscard]] size_type count() const
	{
		std::unique_lock<std::mutex> lg{ _mtx };
		return _cnt;
	}

private:
	size_type _max_cnt;
	std::atomic<size_type> _cnt;
	std::condition_variable _cv;
	mutable std::mutex _mtx;
};

TIMOTHY_NAMESPACE_END

#endif
