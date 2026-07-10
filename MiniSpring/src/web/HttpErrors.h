#pragma once

#include"../core/Core.h"

#include "HttpError.h"

#include<string>

MINISPRING_API inline HttpError NotFound(std::string message = "Resource not found") {
	return HttpError(404, message);
}

MINISPRING_API inline HttpError BadRequest(std::string message = "Bad request") {
	return HttpError(400, message);
}

MINISPRING_API inline HttpError InternalError(std::string message = "Internal server error") {
	return HttpError(500, message);
}