#pragma once

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>

#include <userver/formats/json/value.hpp>
#include <userver/formats/serialize/to.hpp>

#include "s3_url.hpp"

namespace posts_uservice::models {
inline constexpr std::string_view kPostResponseFields =
    " id ,description, media_url, author_id, author_name, author_avatar_url, "
    "created_at ";

struct PostResponse {
  boost::uuids::uuid id;
  std::optional<std::string> description;
  std::optional<S3Url> media_url;
  std::optional<boost::uuids::uuid> author_id;
  std::string author_name;
  std::optional<S3Url> author_avatar_url;
  std::chrono::system_clock::time_point created_at;
};

userver::formats::json::Value Serialize(const PostResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>);
}  // namespace posts_uservice::models