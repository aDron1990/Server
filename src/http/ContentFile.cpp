#include "ContentFile.h"

ContentFile::ContentFile(){}
ContentFile::ContentFile(Type type_, std::string data_) : type(std::move(type_)), data(std::move(data_)) {}

std::string contentTypeToString(ContentFile::Type type)
{
	switch (type)
	{
	case ContentFile::Type::HTML:
		return "text/html";
	case ContentFile::Type::CSS:
		return "text/css";
	case ContentFile::Type::JS:
		return "text/javascript";
	case ContentFile::Type::JPEG:
		return "image/jpeg";
	case ContentFile::Type::PNG:
		return "image/png";
	default:
		throw;
	}
}

ContentFile::Type stringToContentType(std::string typestr)
{
	if (typestr == "text/html")
		return ContentFile::Type::HTML;
	else if (typestr == "text/css")
		return ContentFile::Type::CSS;
	else if (typestr == "text/javascript")
		return ContentFile::Type::JS;
	else if (typestr == "image/jpeg")
		return ContentFile::Type::JPEG;
	else if (typestr == "image/png")
		return ContentFile::Type::PNG;
}

ContentFile::Type extentionToContentType(std::string extention)
{
	if (extention == ".html")
		return ContentFile::Type::HTML;
	else if (extention == ".css")
		return ContentFile::Type::CSS;
	else if (extention == ".js")
		return ContentFile::Type::JS;
	else if (extention == ".jpeg" || extention == ".jpg")
		return ContentFile::Type::JPEG;
	else if (extention == ".png")
		return ContentFile::Type::PNG;
}