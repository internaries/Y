#include "last_posts.hpp"

#include <fmt/format.h>
#include <boost/uuid/uuid.hpp>
#include <chrono>
#include <stdexcept>
#include <string>
#include <userver/components/component_context.hpp>

#include <userver/formats/json/serialize.hpp>
#include <userver/formats/json/value_builder.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>

#include "models/post.hpp"
#include "userver/storages/secdist/exceptions.hpp"
#include "utils/errors.hpp"
#include "utils/fields.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/storages/postgres/result_set.hpp>
#include <userver/utils/datetime.hpp>

namespace posts_uservice {

int ParseSizeArgument(const std::string& size_argument) {
  if (size_argument.empty()) {
    return POSTS_USERVICE_LAST_POSTS_DEFAULT_PAGE_SIZE;
  }

  std::size_t pos = 0;
  int result;
  try {
    result = std::stoi(size_argument, &pos);
    if (pos != size_argument.size()) {
      throw std::invalid_argument("");
    }
    if (result < 1 || result > 100) {
      throw std::out_of_range("");
    }
  } catch (const std::out_of_range&) {
    throw errors::ValidationException("size", "Size of page is limited in range from 1 to 100");
  } catch (const std::invalid_argument&) {
    throw errors::ValidationException("size", "Int is expected");
  }
  return result;
}

std::chrono::system_clock::time_point ParsePageArgument(const std::string& page_argument) {
  try {
    return userver::utils::datetime::Stringtime(page_argument, userver::utils::datetime::kDefaultTimezone,
                                                userver::utils::datetime::kRfc3339Format);
  } catch (const userver::utils::datetime::DateParseError&) {
    throw errors::ValidationException("page", "Page token is not valid");
  }
}

namespace {

class LastPosts final : public userver::server::handlers::HttpHandlerBase {
  userver::storages::postgres::ResultSet GetPostsFromDb(const std::string& page_argument,
                                                        const boost::uuids::uuid& user_id, int size) const {
    if (page_argument.empty()) {
      return pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                  fmt::format("SELECT {} from posts WHERE author_id = $1 "
                                              "ORDER BY created_at DESC "
                                              "LIMIT $2 ",
                                              models::kPostResponseFields),
                                  user_id, size);

    } else {
      std::chrono::system_clock::time_point page = ParsePageArgument(page_argument);
      return pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                  fmt::format("SELECT {} from posts WHERE author_id = $1 "
                                              "AND created_at < $3 "
                                              "ORDER BY created_at DESC "
                                              "LIMIT $2 ",
                                              models::kPostResponseFields),
                                  user_id, size, page);
    }
  }

 public:
  static constexpr std::string_view kName = "handler-last-posts";

  LastPosts(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    const std::string& user_id_argument = request.GetPathArg("userId");
    boost::uuids::uuid user_id = utils::ParseUUIDArgument(user_id_argument);

    const std::string& page_argument = request.GetArg("page");

    const std::string& size_argument = request.GetArg("size");
    int size = ParseSizeArgument(size_argument);

    auto res = GetPostsFromDb(page_argument, user_id, size);

    auto posts = res.AsSetOf<models::PostResponse>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder response;
    response["posts"] = posts;
    if (!posts.IsEmpty()) {
      response["nextPage"] = response["posts"][posts.Size() - 1]["createdAt"];
    }

    return userver::formats::json::ToPrettyString(response.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendLastPosts(userver::components::ComponentList& component_list) { component_list.Append<LastPosts>(); }

}  // namespace posts_uservice
