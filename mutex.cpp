#include "mutex.h"
#include <iostream>
#include <cassert>

void my::lock_graph::add_vertex(vertex *v) {
    holded_mutexes[std::this_thread::get_id()].insert(v);
}

void my::lock_graph::delete_vertex(vertex *v) {
    holded_mutexes[std::this_thread::get_id()].erase(v);
}

void my::lock_graph::add_edges(vertex *v) {
    for (vertex* vout : holded_mutexes[std::this_thread::get_id()]) {
        vout->next_vertex = v;
    }
}

void my::lock_graph::delete_edges() {
    for (vertex *v : holded_mutexes[std::this_thread::get_id()]) {
        v->next_vertex = nullptr;
    }
}

bool my::lock_graph::loop_found(vertex *starting_vertex) const {
    vertex* current_vertex = starting_vertex->next_vertex;
    while(current_vertex != starting_vertex) {
        if (!current_vertex) {
            return false;
        }
        current_vertex = current_vertex->next_vertex;
    }
    return true;
}

my::lock_graph my::mutex::graph = lock_graph();

my::mutex::mutex() {
    v = new vertex();
}

my::mutex::~mutex() {
    delete v;
}

void my::mutex::lock() {
    if (m.try_lock()) {
        graph.delete_edges();
        graph.add_vertex(v);
    } else {
        graph.add_edges(v);
        if (graph.loop_found(v)) {
            std::cout << "DEADLOCK\n";
            assert(0);
        }
    }
}

void my::mutex::unlock() {
    graph.delete_vertex(v);
    m.unlock();
}

bool my::mutex::try_lock() {
    if (m.try_lock()) {
        graph.delete_edges();
        graph.add_vertex(v);
        return true;
    }
    return false;
}

my::mutex m1, m2;
void func() {
    for (int i = 0; i < 10000; ++i) {
        m1.lock();
        m2.lock();
        m1.unlock();
        m2.unlock();
    }
}


int main() {


    std::thread t1([&]() {
        for (int i = 0; i < 10000; ++i) {
            m1.lock();
            m2.lock();
            m1.unlock();
            m2.unlock();
        }
    });
    std::thread t2([&]() {
      for (int i = 0; i < 10000; ++i) {
          m2.lock();
          m1.lock();
          m2.unlock();
          m1.unlock();
      }
    });
    t1.join();
    t2.join();
}
