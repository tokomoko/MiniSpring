#pragma once

#include<winsock2.h>
#include<queue>
#include<mutex>
#include<condition_variable>

class RequestQueue {
public:
	void push(SOCKET client) {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_queue.push(client);
		}
		m_cv.notify_one();
	}

	bool pop(SOCKET& out) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.wait(lock, [this] { return m_shutdown || !m_queue.empty(); });

		if (m_queue.empty())
			return false;

		out = m_queue.front();
		m_queue.pop();
		return true;
	}

	void shutdown() {
		{
			std::lock_guard<std::mutex> lock(m_mutex);
			m_shutdown = true;
		}
		m_cv.notify_all();
	}

private:
	std::queue<SOCKET> m_queue;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	bool m_shutdown = false;
};