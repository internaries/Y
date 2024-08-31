#include "create_post.hpp"

#include <fmt/format.h>
#include <cstddef>
#include <optional>
#include <string>
#include <tuple>
#include <userver/clients/http/client.hpp>
#include <userver/clients/http/config.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_context.hpp>

#include <userver/components/minimal_server_component_list.hpp>
#include <userver/engine/task/task_processor_fwd.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/utils/async.hpp>
#include <userver/utils/daemon_run.hpp>

#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>

#include "models/post.hpp"
#include "models/s3_url.hpp"
#include "userver/storages/secdist/exceptions.hpp"

#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include "utils/errors.hpp"
#include "utils/fields.hpp"

namespace posts_uservice {

bool isSymbol(char c) { return c != ' ' && c != '\n'; }

std::string FormatText(const std::string& input) {
  std::string result = "";

  for (const auto& c : input) {
    if (isSymbol(c)) {
      result.push_back(c);
      continue;
    }

    if (result.empty()) {
      continue;
    }

    if (c == '\n' && result.back() == ' ') {
      result.back() = c;
      continue;
    }

    if (isSymbol(result.back())) {
      result.push_back(c);
    }
  }

  if (!result.empty() && !isSymbol(result.back())) {
    result.pop_back();
  }

  return result;
}

namespace {

class CreatePost final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-create-post";

  CreatePost(const userver::components::ComponentConfig& config, const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()),
        http_client_{context.FindComponent<userver::components::HttpClient>().GetHttpClient()} {}

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(const userver::server::http::HttpRequest& request,
                                 userver::server::request::RequestContext&) const override {
    const auto& user_id_argument = request.GetHeader("System-Design-User-Id");
    // should throw 401
    auto author_id = utils::ParseUUIDArgument(user_id_argument);

    const auto& description = FormatText(request.GetArg("description"));
    const auto& media = request.GetArg("media");

    // here u need create field in json with error
    if (description.length() > 280) {
      throw errors::ValidationException("description", "Text is too big, max length is 280");
    }

    // while we can't add media
    if (description.empty()) {
      throw errors::ValidationException("description", "Is no any text");
    }

    // if media isn't image u need send bad request

    // userver::server::http::HttpRequest S3Request();
    //  const auto& url = "http://storage.yandexcloud.net/y-media/<ключ_объекта>?<query-параметры>";
    //  const auto& data = "data";

    // auto external_response =  http_client_.CreateRequest().put(url, data);

    auto getting_user = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                                             "SELECT name, avatar_url FROM users WHERE id = $1", author_id);

    // throw errors::ValidationException("description", "log");

    auto [author_name, author_avatar_url] =
        getting_user.AsSingleRow<std::tuple<std::string, models::S3Url>>(userver::storages::postgres::kRowTag);

    auto result_create = pg_cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO posts (author_id, author_name, author_avatar_url, description) VALUES ($1, $2, $3, $4) "
        "RETURNING id, created_at;",
        author_id, author_name, author_avatar_url, description);

    auto [post_id, created_at] =
        result_create.AsSingleRow<std::tuple<boost::uuids::uuid, std::chrono::system_clock::time_point>>(
            userver::storages::postgres::kRowTag);

    models::PostResponse pr{post_id, description, std::nullopt, author_id, author_name, author_avatar_url, created_at};

    userver::formats::json::ValueBuilder builder(pr);

    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    return ToPrettyString(builder.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
  userver::clients::http::Client& http_client_;
};

}  // namespace

void AppendCreatePost(userver::components::ComponentList& component_list) { component_list.Append<CreatePost>(); }

}  // namespace posts_uservice
