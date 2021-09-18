#include "mutex.h"
#include <thread>
#include <unordered_map>
#include <iostream>

void my::mutex::lock() {}

void my::mutex::unlock() {}

bool my::mutex::try_lock() {}

int main() {
    std::unordered_map<std::thread::id, int> mp;
    mp[std::this_thread::get_id()] = 5;
    for (auto x : mp) {
        std::cout << x.first << ' ' << x.second;
    }
}
