# Y
System Design Proof-of-Concept Twitter App

It utilizes a fan-out write strategy, meaning that all posts are immediately added to the feeds of users upon creation. 

Written on C++ using [userver](https://userver.tech/) framework, postgresql, redis and s3 storage.

# Authors
* [Dmitry Deruzhinsky](https://github.com/terebellum) 
* [Sergey Vinogradov](https://github.com/St1lk3r)
* [@timuur](https://github.com/tiimuur)

# Roadmap
Here are some of the major upcoming features. You can view the complete roadmap with todos [here](https://github.com/orgs/internaries/projects/1). 


-  Add feeds
-  Add creation support to S3 media
-  Add redis cache for feeds
-  Add authorization
-  Add web client