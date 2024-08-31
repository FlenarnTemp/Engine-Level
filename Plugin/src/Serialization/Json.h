#pragma once

#include <json/json.h>

inline RE::BSResourceNiBinaryStream& operator>>(
	RE::BSResourceNiBinaryStream& a_sin,
	Json::Value& a_root)
{
	Json::CharReaderBuilder fact;
	std::unique_ptr<Json::CharReader> const reader{ fact.newCharReader() };

	std::uint32_t size = a_sin.stream->totalSize;
	std::unique_ptr<char[]> buffer = std::make_unique<char[]>(size);
	a_sin.read(buffer.get(), size);

	std::unique_ptr<char[]>::pointer begin = buffer.get();
	std::unique_ptr<char[]>::pointer end = begin + size;

	std::string errors;
	bool ok = reader->parse(begin, end, std::addressof(a_root), std::addressof(errors));

	if (!ok)
	{
		throw std::runtime_error(errors);
	}

	return a_sin;
}

