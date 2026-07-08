#pragma once

#include "HttpError.h"

#include<string>

inline HttpError NotFound(std::string message = "Resource not found") {
	return HttpError(404, message);
}

inline HttpError BadRequest(std::string message = "Bad request") {
	return HttpError(400, message);
}

inline HttpError InternalError(std::string message = "Internal server error") {
	return HttpError(500, message);
}