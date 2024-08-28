#include "fields.hpp"
#include <optional>
#include "consts.hpp"
#include "utils/errors.hpp"

namespace posts_uservice::utils {
boost::uuids::uuid ParseUUIDArgument(const std::string& uuid_argument, const std::string& field_name) {
  try {
    return boost::lexical_cast<boost::uuids::uuid>(uuid_argument);
  } catch (const std::bad_cast& e) {
    throw errors::ValidationException(field_name, e.what());
  }
}

std::optional<std::chrono::system_clock::time_point> ParsePaginationArgument(const std::string& page_argument) {
  if (page_argument.empty()) {
    return std::nullopt;
  }
  try {
    return userver::utils::datetime::Stringtime(page_argument, userver::utils::datetime::kDefaultTimezone,
                                                userver::utils::datetime::kRfc3339Format);
  } catch (const userver::utils::datetime::DateParseError&) {
    throw errors::ValidationException("page", "Page token is not valid");
  }
}

int ParsePaginationSizeArgument(const std::string& size_argument) {
  if (size_argument.empty()) {
    return POSTS_USERVICE_PAGINATION_DEFAULT_PAGE_SIZE;
  }

  std::size_t pos = 0;
  int result;
  try {
    result = std::stoi(size_argument, &pos);
    if (pos != size_argument.size()) {
      throw std::invalid_argument("");
    }
    if (result < POSTS_USERVICE_PAGINATION_MIN_PAGE_SIZE || result > POSTS_USERVICE_PAGINATION_MAX_PAGE_SIZE) {
      throw std::out_of_range("");
    }
  } catch (const std::out_of_range&) {
    throw errors::ValidationException("size", "Size of page is limited in range from 1 to 100");
  } catch (const std::invalid_argument&) {
    throw errors::ValidationException("size", "Int is expected");
  }
  return result;
}

}  // namespace posts_uservice::utils