#pragma once

#include"../core/Core.h"
#include"Filter.h"

#include<memory>
#include<vector>

class MINISPRING_API FilterChain {
public:
	void add(std::unique_ptr<IFilter> filter) {
		m_filters.push_back(std::move(filter));
	}

	bool run(HttpRequest& request, HttpResponse& response) {
		for (auto& filter : m_filters)
			if (!filter->doFilter(request, response))
				return false;
		return true;
	}

private:
	std::vector<std::unique_ptr<IFilter>> m_filters;
};