DROP TABLE IF EXISTS exercise cascade;
DROP TABLE IF EXISTS workout cascade;


CREATE TABLE exercise(
    id bigint PRIMARY KEY,
    name text
);


CREATE TABLE workout(
    id SERIAL PRIMARY KEY,
    date date,
    exercise bigint NOT NULL,
    reps varchar(32),
    FOREIGN KEY (exercise) REFERENCES exercise (id) ON DELETE CASCADE
);
