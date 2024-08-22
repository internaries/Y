#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace posts_uservice::utils {
boost::uuids::uuid ParseUUIDArgument(const std::string& uuid_argument, const std::string& field_name = "uuid");
}