#include "s3_url.hpp"

#include <userver/utest/utest.hpp>

namespace posts_uservice::models {
UTEST(S3Url, ToLink) {
  S3Url data{"bucket", "key"};
  EXPECT_EQ(data.ToLink(), "https://storage.yandexcloud.net/" + data.bucket + "/" + data.key);
}
}  // namespace posts_uservice::models
