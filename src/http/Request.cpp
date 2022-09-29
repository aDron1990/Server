#include "Request.h"

Request::Request() {}
Request::Request(Method method_, std::string url_, std::string body_) :
	method(std::move(method_)), url(std::move(url_)), body(std::move(body_)) {}

const Request parseRequest(std::string requestCode)
{
	Request result;
	if (requestCode.find("GET") != std::string::npos)
	{
		result.method = Request::Method::Get;
		int p1 = requestCode.find("GET") + 4;
		int p2 = (requestCode.find("HTTP/1.1") - 1) - p1;
		result.url = requestCode.substr(p1, p2);
	}
	else if ((requestCode.find("POST") != std::string::npos))
	{
		result.method = Request::Method::Post;
	}

	return result;
}