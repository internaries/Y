#pragma once

#include <userver/storages/postgres/io/io_fwd.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>

#include <string>

namespace posts_uservice::models {
struct S3Url {
  std::string bucket, key;

  std::string ToLink() const;
};

}  // namespace posts_uservice::models

template <>
struct userver::storages::postgres::io::CppToUserPg<posts_uservice::models::S3Url> {
  static constexpr userver::storages::postgres::DBTypeName postgres_name = "public.s3_url";
};
