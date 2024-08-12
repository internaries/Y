CREATE TABLE IF NOT EXISTS users (
    ID SERIAL PRIMARY KEY,
    NAME TEXT NOT NULL, -- probably CHARACTER VARYING(CONST) will be better than TEXT
    LOGIN TEXT UNIQUE NOT NULL,
    PASSWORD TEXT NOT NULL,
    AVATAR_ID INT,
    DESCRIPTION TEXT,
    CREATED_AT TIMESTAMP NOT NULL
);

CREATE TABLE IF NOT EXISTS posts (
    ID SERIAL PRIMARY KEY,
    USER_ID INT,
    MEDIA_ID INT,
    DESCRIPTION TEXT,
    CREATED_AT TIMESTAMP NOT NULL
);

CREATE TABLE IF NOT EXISTS likes (
    ID SERIAL PRIMARY KEY,
    USER_ID INT, -- WHO LIKED
    POST_ID INT -- WHAT LIKED
);

CREATE TABLE IF NOT EXISTS follows (
    ID SERIAL PRIMARY KEY,
    FOLOWEE_ID INT, -- to whom
    FOLOWER_OD INT -- who
);