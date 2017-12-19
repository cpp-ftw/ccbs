#include <ccbs/util/parallel.hpp>

#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>
#include <condition_variable>

enum class parallel_state
{
    none,
    started,
    succeeded,
    failed
};

enum class thread_status
{
    working,
    finished,
    blocked,
};

int parallel_process(std::size_t count, std::size_t no_threads, std::function<int(size_t)> callback,
                     std::function<bool(size_t, size_t)> conflicts)
{
    std::vector<parallel_state> states(count);
    for (auto& state : states)
        state = parallel_state::none;

    int exit_code = 0;
    std::mutex state_mtx;
    std::condition_variable block;


    auto thread_func = [&]() -> void
    {
        thread_status state = thread_status::working;
        while (state != thread_status::finished)
        {
            size_t index = 0;
            {
                std::lock_guard<std::mutex> lock{state_mtx};
                state = thread_status::finished;
                for (size_t i = 0; i < states.size() && state == thread_status::finished; ++i)
                {
                    if (states[i] == parallel_state::failed)
                    {
                        return;
                    }
                    else if (states[i] == parallel_state::none)
                    {
                        auto * first = &states[0];
                        auto currentIt = states.begin() + i;

                        bool blocked = std::any_of(states.begin(), currentIt, [&](parallel_state const& state1){
                            return state1 != parallel_state::succeeded && conflicts(&state1 - first, i);
                        });

                        if (blocked)
                        {
                            state = thread_status::blocked;
                        }
                        else
                        {
                            index = i;
                            state = thread_status::working;
                        }
                    }
                }
            }
            if (state == thread_status::working)
            {
                {
                    std::lock_guard<std::mutex> lock{state_mtx};
                    states[index] = parallel_state::started;
                }

                int result = callback(index);

                {
                    std::lock_guard<std::mutex> lock{state_mtx};
                    if (result == 0)
                    {
                        states[index] = parallel_state::succeeded;
                    }
                    else
                    {
                        states[index] = parallel_state::failed;
                        exit_code = result;
                        return;
                    }
                }
                block.notify_all();
            }
            else if (state == thread_status::blocked)
            {
                std::unique_lock<std::mutex> lk{state_mtx};
                block.wait(lk, [] { return true; });
            }
        }
    };

    std::vector<std::thread> threads;
    threads.reserve(no_threads);
    for (size_t i = 0; i < no_threads; ++i)
        threads.emplace_back(thread_func);

    for (size_t i = 0; i < no_threads; ++i)
        threads[i].join();

    return exit_code;
}
