#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>
#include <boost/uuid/uuid.hpp>
#include <userver/storages/postgres/io/pg_types.hpp>
#include <userver/storages/postgres/io/io_fwd.hpp>


namespace posts_uservice {
    struct S3_url {
        std::string bucket,key;
    };
    struct RowResponse {
        boost::uuids::uuid id;
        std::optional<std::string> description;
        std::optional<S3_url> media_url;
        std::optional<boost::uuids::uuid> author_id;
        std::string author_name;
        std::optional<S3_url> author_avatar_url;
        std::chrono::system_clock::time_point created_at;
    };

    std::string ToLink(const S3_url& url);


void AppendCreatePost(userver::components::ComponentList &component_list);

} // namespace service_template

template <>
struct userver::storages::postgres::io::CppToUserPg<posts_uservice::S3_url> {
    static constexpr userver::storages::postgres::DBTypeName postgres_name = "public.s3_url";
};
