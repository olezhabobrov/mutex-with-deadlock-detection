#pragma once
#include <mutex>
#include <unordered_map>
#include <vector>

namespace my {
class graph {

};

class mutex {
    std::mutex mutex;

public:
    void lock();
    void unlock();
    bool try_lock();
};

} // namespace my