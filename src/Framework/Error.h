#pragma once

#include <string>

namespace Error {

static inline void ASSERT(bool condition, const std::string& message = "")
{
	if (condition)
		return;

	std::printf(message.c_str());
	abort();
}
}