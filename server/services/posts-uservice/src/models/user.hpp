#pragma once

#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <optional>

#include <userver/formats/json/value.hpp>
#include <userver/formats/serialize/to.hpp>

namespace posts_uservice::models {

struct UserResponse {
  boost::uuids::uuid id;
};

userver::formats::json::Value Serialize(const UserResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>);

}  // namespace posts_uservice::models