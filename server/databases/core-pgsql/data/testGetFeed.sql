INSERT INTO users(id, name, login, password) 
VALUES ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', 'user1', 'pass'),
       ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', 'user2', 'pass'),
       ('12cea0f9-cde4-4ae8-8548-b7af307a9680', 'user3', 'user3', 'pass')
ON CONFLICT DO NOTHING;

INSERT INTO feed(author_id, author_name, description, created_at, owner_id)
VALUES  ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '1', '2024-08-17T18:50:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680' ),
        ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', '-1', '2024-08-17T18:40:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '2', '2024-08-17T18:40:20.711473+00:00' ,'12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', '-1', '2024-08-17T18:30:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '-1', '2024-08-17T18:20:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', '3', '2024-08-17T18:10:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '-1', '2024-08-17T18:00:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', '-1', '2024-08-17T17:40:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '-1', '2024-08-17T17:20:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('1e755517-eea9-414a-acb1-7a84600898bd', 'user2', '4', '2024-08-17T17:10:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '-1', '2024-08-17T16:40:20.711473+00:00', '1e755517-eea9-414a-acb1-7a84600898bd'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '5', '2024-08-17T15:40:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '6', '2024-08-17T14:40:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '7', '2024-08-17T12:40:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '8', '2024-08-17T10:40:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '9', '2024-08-17T10:20:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680'),
        ('c25a7e41-8a27-48ca-bbaf-dc27a000b9ba', 'user1', '10', '2024-08-17T10:00:20.711473+00:00', '12cea0f9-cde4-4ae8-8548-b7af307a9680')
ON CONFLICT DO NOTHING;