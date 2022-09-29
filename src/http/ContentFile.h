#pragma once

#include <string>

class ContentFile
{
public:

	enum class Type
	{
		HTML,
		CSS,
		JS,
		JPEG,
		PNG
	};

	Type type;
	std::string data;

	ContentFile();
	ContentFile(Type type_, std::string data_);
};

std::string contentTypeToString(ContentFile::Type type);
ContentFile::Type stringToContentType(std::string typestr);
ContentFile::Type extentionToContentType(std::string extention);