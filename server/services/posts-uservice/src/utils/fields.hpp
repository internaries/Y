#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <chrono>
#include <optional>

namespace posts_uservice::utils {

boost::uuids::uuid ParseUUIDArgument(const std::string& uuid_argument, const std::string& field_name = "uuid");
int ParsePaginationSizeArgument(const std::string& size_argument);
std::optional<std::chrono::system_clock::time_point> ParsePaginationArgument(const std::string& page_argument);
}  // namespace posts_uservice::utils