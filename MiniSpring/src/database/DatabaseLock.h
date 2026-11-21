#pragma once

#include <mutex>

class DatabaseLock {
 public:
  static std::recursive_mutex &mutex() {
    static std::recursive_mutex m;
    return m;
  }
};

class DatabaseGuard {
 public:
  DatabaseGuard() : m_lock(DatabaseLock::mutex()) {}

 private:
  std::lock_guard<std::recursive_mutex> m_lock;
};