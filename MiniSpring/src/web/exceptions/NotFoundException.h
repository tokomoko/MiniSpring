#pragma once

#include"HttpException.h"

class NotFoundException : public HttpException
{
public:
	NotFoundException(std::string message) : HttpException(404, std::move(message)) {}
};