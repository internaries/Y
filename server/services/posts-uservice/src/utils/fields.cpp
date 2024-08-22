#include "fields.hpp"
#include "utils/errors.hpp"

namespace posts_uservice::utils {
boost::uuids::uuid ParseUUIDArgument(const std::string& uuid_argument, const std::string& field_name) {
  try {
    return boost::lexical_cast<boost::uuids::uuid>(uuid_argument);
  } catch (const std::bad_cast& e) {
    throw errors::ValidationException(field_name, e.what());
  }
}
}  // namespace posts_uservice::utils