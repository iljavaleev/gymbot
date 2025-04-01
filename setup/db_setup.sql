DROP TABLE IF EXISTS workout cascade;

CREATE TABLE workout(
    id SERIAL PRIMARY KEY,
    exercise TEXT,
    reps varchar(32),
    date date NOT NULL,
    prev date,
    next date
);
