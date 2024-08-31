#include "user.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace posts_uservice::models {

userver::formats::json::Value Serialize(const UserResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;

  builder["id"] = boost::uuids::to_string(data.id);

  return builder.ExtractValue();
}
}  // namespace posts_uservice::models