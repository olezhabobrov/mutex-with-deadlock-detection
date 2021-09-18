#pragma once
#include <mutex>
#include <unordered_map>
#include <set>
#include <thread>

namespace my {
struct vertex {
    vertex *next_vertex = nullptr;
};

class lock_graph {
    std::unordered_map<std::thread::id,
                       std::set<vertex*>> holded_mutexes;

public:
    void add_vertex(vertex *);
    void delete_vertex(vertex *);
    void add_edges(vertex *);
    void delete_edges();
    bool loop_found(vertex *) const;
};

class mutex {
    static lock_graph graph;
    vertex *v = nullptr;
    std::mutex m;

public:
    void lock();
    void unlock();
    bool try_lock();

    mutex();
    ~mutex();
};

} // namespace my