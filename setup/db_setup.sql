DROP TABLE IF EXISTS mens_workout;
DROP TABLE IF EXISTS womens_workout;

CREATE TABLE mens_workout(
    id SERIAL PRIMARY KEY,
    exercise TEXT,
    reps varchar(32),
    date date NOT NULL,
    prev date,
    next date
);

CREATE TABLE womens_workout(
    id SERIAL PRIMARY KEY,
    exercise TEXT,
    reps varchar(32),
    date date NOT NULL,
    prev date,
    next date
);