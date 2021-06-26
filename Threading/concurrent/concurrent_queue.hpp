#pragma once

#ifndef CONCURRENT_QUEUE_HPP

#define CONCURRENT_QUEUE_HPP

#include <timothy.h>

#include <utility>
#include <queue>
#include <mutex>
#include <optional>

TIMOTHY_CONCURRENT_NAMESPACE_BEGIN

template <typename Elem_type>
class concurrent_queue
{
public:
	using elem_type = Elem_type;
	using size_type = typename std::queue<elem_type>::size_type;

	concurrent_queue() = default;
	concurrent_queue(const concurrent_queue&) = delete;

	template <typename... Ts>
	void emplace(Ts... args)
	{
		std::lock_guard<std::mutex> lg{ _mtx };
		_q.emplace(std::forward<Ts>(args)...);
	}

	[[nodiscard]] std::optional<elem_type> popfront()
	{
		std::lock_guard<std::mutex> lg{ _mtx };
		if (_q.empty()) return std::nullopt;
		elem_type ret = std::move(_q.front());
		_q.pop();
		return ret;
	}

	[[nodiscard]] bool empty() const
	{
		std::lock_guard<std::mutex> lg{ _mtx };
		return _q.empty();
	}

	[[nodiscard]] auto size() const
	{
		std::lock_guard<std::mutex> lg{ _mtx };
		return _q.size();
	}

	

private:
	std::queue<elem_type> _q;
	mutable std::mutex _mtx;
};

TIMOTHY_CONCURRENT_NAMESPACE_END

#endif // !CONCURRENT_QUEUE_HPP

