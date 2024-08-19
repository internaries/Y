CREATE EXTENSION pgcrypto;

CREATE INDEX IF NOT EXISTS idx_created_at ON posts USING btree (created_at DESC);

CREATE TYPE IF NOT EXISTS s3_url AS ( 
    bucket TEXT,
    key TEXT
);

CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(32) NOT NULL, 
    login VARCHAR(32) UNIQUE NOT NULL,
    password VARCHAR(32) NOT NULL,
    avatar_url S3_URL,
    description VARCHAR(280),
    created_at TIMESTAMP DEFAULT NOW() 
);

CREATE TABLE IF NOT EXISTS posts (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    author_id UUID,
    author_name VARCHAR(32) NOT NULL, 
    author_avatar_url S3_URL,
    media_url S3_URL,
    description VARCHAR(280),
    created_at TIMESTAMP DEFAULT NOW(),
    CONSTRAINT fk_author FOREIGN KEY(author_id) REFERENCES users(id)
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

CREATE TYPE s3_url AS ( 
    bucket TEXT,
    key TEXT
);

CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    name VARCHAR(32) NOT NULL, 
    login VARCHAR(32) UNIQUE NOT NULL,
    password VARCHAR(32) NOT NULL,
    avatar_url S3_URL,
    description VARCHAR(280),
    created_at TIMESTAMP NOT NULL DEFAULT NOW() 
);

CREATE TABLE IF NOT EXISTS posts (
    id UUID PRIMARY KEY DEFAULT gen_random_uuid(),
    author_id UUID,
    author_name VARCHAR(32) NOT NULL, 
    author_avatar_url S3_URL,
    media_url S3_URL,
    description VARCHAR(280),
    created_at TIMESTAMP NOT NULL DEFAULT NOW(),
    CONSTRAINT fk_author FOREIGN KEY(author_id) REFERENCES users(id)
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