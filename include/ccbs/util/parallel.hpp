#ifndef PROJECT_PARALLEL_HPP
#define PROJECT_PARALLEL_HPP

#include <cstddef>
#include <functional>

int parallel_process(
    std::size_t count,
    std::size_t no_threads,
    std::function<int(std::size_t)> callback,
    std::function<bool(std::size_t, std::size_t)> conflicts);

template<
    typename RandomAccessIterator,
    typename Callback,
    typename Conflicts>
int parallel_process(
    std::size_t no_threads,
    RandomAccessIterator first,
    RandomAccessIterator last,
    Callback&& callback,
    Conflicts&& conflicts)
{
    return parallel_process(
        last - first,
        no_threads,
        [&](std::size_t i) -> int { return callback(*(first + i)); },
        [&](std::size_t a, std::size_t b) -> bool { return conflicts(first[a], first[b]); });
}

#endif //PROJECT_PARALLEL_HPP
