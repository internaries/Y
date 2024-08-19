#include "get_post.hpp"

#include <boost/lexical_cast.hpp>
#include <fmt/format.h>
#include <userver/components/component_context.hpp>

#include <userver/server/handlers/http_handler_base.hpp>

#include "userver/storages/secdist/exceptions.hpp"

#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>


namespace posts_uservice {
std::string ToLink(const S3_url &url) {
  return "https://storage.yandexcloud.net/" + url.bucket + "/" + url.key;

}

namespace {
class GetPost final : public userver::server::handlers::HttpHandlerBase {
public:
  static constexpr std::string_view kName = "handler-get-post";

  GetPost(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& context) : HttpHandlerBase(config,context),
            pg_cluster_(context.FindComponent<userver::components::Postgres>("postgres-db-1").GetCluster()) {

  }

  using HttpHandlerBase::HttpHandlerBase;

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest &request,
      userver::server::request::RequestContext &) const override {
    auto post_id_as_text = request.GetPathArg("postId");
    boost::uuids::uuid post_id;
    try {
      post_id = boost::lexical_cast<boost::uuids::uuid>(post_id_as_text);
    }
    catch (const std::bad_cast& ex) {
      auto& response = request.GetHttpResponse();
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return "Bad Id";
    }

    auto res = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kMaster,"SELECT id ,description, media_url, author_id, author_name, author_avatar_url, created_at from posts WHERE id = $1",post_id);
    if(res.IsEmpty()) {
      auto& response = request.GetHttpResponse();
      response.SetStatus(userver::server::http::HttpStatus::kNotFound);
      return "Id does not exist";
    }
    auto parsed = res.AsSingleRow<RowResponse>(userver::storages::postgres::kRowTag);

    userver::formats::json::ValueBuilder builder;

    builder["id"] = post_id_as_text;
    builder["text"] = parsed.description.value();
    builder["mediaUrl"] = (parsed.media_url.has_value() ? ToLink(parsed.media_url.value()) : "");
    builder["authorId"] = (parsed.author_id.has_value() ? boost::uuids::to_string(parsed.author_id.value()) : "");
    builder["authorName"] = parsed.author_name;
    builder["authorAvatarUrl"] = (parsed.author_avatar_url.has_value() ? ToLink(parsed.author_avatar_url.value()) : "");
    builder["createdAt"] = parsed.created_at;



    auto& response = request.GetHttpResponse();
    response.SetStatus(userver::server::http::HttpStatus::kOk);
    return ToPrettyString(builder.ExtractValue());



  }
private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

} // namespace

void AppendGetPost(userver::components::ComponentList &component_list) {
  component_list.Append<GetPost>();
}

} // namespace service_template
