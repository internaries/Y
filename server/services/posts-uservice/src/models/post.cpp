#include "post.hpp"

#include <boost/uuid/uuid_io.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace posts_uservice::models {

userver::formats::json::Value Serialize(const PostResponse& data,
                                        userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;

  builder["id"] = boost::uuids::to_string(data.id);
  builder["text"] = data.description.value();
  builder["mediaUrl"] = (data.media_url.has_value() ? data.media_url.value().ToLink() : "");
  builder["authorId"] = (data.author_id.has_value() ? boost::uuids::to_string(data.author_id.value()) : "");
  builder["authorName"] = data.author_name;
  builder["authorAvatarUrl"] = (data.author_avatar_url.has_value() ? data.author_avatar_url.value().ToLink() : "");
  builder["createdAt"] = data.created_at;

  return builder.ExtractValue();
}
}  // namespace posts_uservice::models