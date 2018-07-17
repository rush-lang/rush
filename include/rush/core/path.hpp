#pragma once

#ifndef RUSH_CORE_PATH_HPP
#define RUSH_CORE_PATH_HPP


#include <string_view>

namespace rush {
	namespace filesystem {
		class path {
		public:
			path filename() const noexcept;
			path directory() const noexcept;
			std::string_view extension() const noexcept;
		};
	}
}

#endif // RUSH_CORE_PATH_HPP
