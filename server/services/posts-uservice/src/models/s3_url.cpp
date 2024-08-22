#include "s3_url.hpp"

namespace posts_uservice::models {
std::string S3Url::ToLink() const { return "https://storage.yandexcloud.net/" + bucket + "/" + key; }
}  // namespace posts_uservice::models
