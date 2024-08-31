INSERT INTO users(id, name, login, password) 
VALUES ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', 'user1', 'pass'),
       ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', 'user2', 'pass'),
       ('267d54d2-8956-4045-9885-1bf238fc90ce', 'user3', 'user3', 'pass'),
       ('eacea82c-b498-4545-8db2-04895f194962', 'user4', 'user4', 'pass')
ON CONFLICT DO NOTHING;

INSERT INTO follows(folowee_id, folower_id) 
VALUES ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', '1e755517-eea9-414a-acb1-7a84600898bd'),
       ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', '267d54d2-8956-4045-9885-1bf238fc90ce'),
       ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'eacea82c-b498-4545-8db2-04895f194962'),
       ('267d54d2-8956-4045-9885-1bf238fc90ce', 'c25a7e41-8a27-48ca-bbaf-dc27a000b9ba'),
       ('267d54d2-8956-4045-9885-1bf238fc90ce', '1e755517-eea9-414a-acb1-7a84600898bd'),
       ('eacea82c-b498-4545-8db2-04895f194962', '267d54d2-8956-4045-9885-1bf238fc90ce')
ON CONFLICT DO NOTHING;
