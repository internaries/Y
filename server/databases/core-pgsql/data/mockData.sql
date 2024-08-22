INSERT INTO users(id, name, login, password, avatar_url) VALUES ('46775165-7542-49d9-a3b2-a7878980a2bc', 'kittycat', 'cat', 'password', ('y-media', 'cat_cure_blabla.jpg'))
ON CONFLICT DO NOTHING;

INSERT INTO users(id, name, login, password) VALUES ('6cca3ced-9615-4e2c-bae4-580c86d78710', 'cool', 'catness', 'password')
ON CONFLICT DO NOTHING;

INSERT INTO posts(id,description,media_url,author_id,author_name,author_avatar_url,created_at)
VALUES ('749e5dc8-4c3b-4158-95c1-dc2ff9aab17b','Hehe silly', ('y-media', 'cat_cute_blabla.jpg'),'46775165-7542-49d9-a3b2-a7878980a2bc','kittycat', ('y-media', 'cat_cute_blabla.jpg'),'2024-08-17T18:50:20.711473+00:00')
ON CONFLICT DO NOTHING;

INSERT INTO posts(id,description,media_url,author_id,author_name,author_avatar_url,created_at)
VALUES ('dd1870ab-fb0d-4b79-a890-ce62a091b77a','Just settin up my YYYYYY', NULL,'6cca3ced-9615-4e2c-bae4-580c86d78710','cool', NULL,'2024-08-17T18:29:16.307175+00:00')
ON CONFLICT DO NOTHING;