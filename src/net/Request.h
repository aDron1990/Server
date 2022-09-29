#pragma once

#include <string>

class Request
{
public:

	enum class Method
	{
		Get,
		Post
	};
	
	Method method;
	std::string url;
	std::string body;

	Request();
	Request(Method method_, std::string url_, std::string body_);
};

const Request parseRequest(std::string requestCode);