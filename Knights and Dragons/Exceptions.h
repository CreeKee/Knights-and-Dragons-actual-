#pragma once
#include <exception>
#include <string>

class Exceptions : public std::exception
{
public:
	Exceptions(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* getType() const noexcept;
	int getLine() const noexcept;
	const std::string& getFile() const noexcept;
	std::string getOriginString() const noexcept;
private:
	int line;
	std::string file;
	std::exception _e;
protected:
	mutable std::string whatBuffer;
};