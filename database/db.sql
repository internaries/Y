CREATE EXTENSION pgcrypto;

CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(32) NOT NULL, 
    login VARCHAR(32) UNIQUE NOT NULL,
    password VARCHAR(32) NOT NULL,
    avatar_id INT,
    description VARCHAR(280),
    created_at TIMESTAMP NOT NULL
);

CREATE TABLE IF NOT EXISTS posts (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    user_id UUID,
    media_id INT,
    description VARCHAR(280),
    created_at TIMESTAMP NOT NULL
);

CREATE TABLE IF NOT EXISTS likes (
    user_id UUID, -- WHO LIKED
    post_id UUID, -- WHAT LIKED
    PRIMARY KEY (user_id, post_id),
    CONSTRAINT fk_user FOREIGN KEY(user_id) REFERENCES users(id),
    CONSTRAINT fk_post FOREIGN KEY(post_id) REFERENCES posts(id)
);

CREATE TABLE IF NOT EXISTS follows (
    folowee_id UUID, -- to whom 
    folower_id UUID, -- who 
    PRIMARY KEY (folowee_id, folower_id),
    CONSTRAINT fk_folowee FOREIGN KEY(folowee_id) REFERENCES users(id),
    CONSTRAINT fk_folower FOREIGN KEY(folower_id) REFERENCES users(id)
);