#include "Exceptions.h"
#include <sstream>


Exceptions::Exceptions(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}

const char* Exceptions::what() const noexcept
{
	std::ostringstream oss;
	oss << getType() << std::endl
		<< getOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Exceptions::getType() const noexcept
{
	return "Chili Exception";
}

int Exceptions::getLine() const noexcept
{
	return line;
}

const std::string& Exceptions::getFile() const noexcept
{
	return file;
}

std::string Exceptions::getOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}