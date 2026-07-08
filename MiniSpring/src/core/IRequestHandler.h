#pragma once

class HttpRequest;
class HttpResponse;

class IRequestHandler {
public:
	virtual ~IRequestHandler() = default;

	virtual bool handle(HttpRequest& request, HttpResponse& response) = 0;
};