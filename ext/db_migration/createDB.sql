CREATE TABLE money_per_period (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    datetime_from INTEGER NOT NULL,
    datetime_to INTEGER NOT NULL,
    driver_id INTEGER NOT NULL,
    total_money REAL NOT NULL
);

CREATE INDEX money_per_period_datetime_from_datetime_to_index ON money_per_period (datetime_from, datetime_to);

CREATE TABLE drivers (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    user_login TEXT NOT NULL,
    name TEXT NOT NULL,
    category TEXT NOT NULL,
    career_start INTEGER NOT NULL,
    birth_year INTEGER NOT NULL,
    FOREIGN KEY (user_login) REFERENCES users (login)
);

CREATE TABLE cars (
    number TEXT PRIMARY KEY,
    brand TEXT NOT NULL,
    initial_mileage REAL NOT NULL,
    capacity REAL NOT NULL
);

CREATE TABLE orders (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    datetime INTEGER NOT NULL,
    driver_id INTEGER NOT NULL,
    car_number TEXT NOT NULL,
    length REAL NOT NULL,
    cargo_weight REAL NOT NULL,
    cost REAL NOT NULL,
    FOREIGN KEY (driver_id) REFERENCES drivers (id),
    FOREIGN KEY (car_number) REFERENCES cars (number)
);

CREATE INDEX orders_driver_id_index ON orders (driver_id);
CREATE INDEX orders_car_number_index ON orders (car_number);
CREATE INDEX orders_driver_id_datetime_index ON orders (driver_id, datetime);

CREATE TABLE users (
    login TEXT PRIMARY KEY,
    password TEXT NOT NULL,
    role TEXT NOT NULL
);

CREATE INDEX users_role_index ON users (role);

INSERT INTO users (login, password, role) VALUES ('vasily324', '1111', 'admin');
INSERT INTO users (login, password, role) VALUES ('oleg324', '187536', 'driver');
INSERT INTO cars (number, brand, initial_mileage, capacity) VALUES ('3089 AB-7', 'BMW', 305608.0, 1500.0);
INSERT INTO cars (number, brand, initial_mileage, capacity) VALUES ('8924 HP-3', 'Audi', 3000.0, 600.0);
INSERT INTO drivers (user_login, name, category, career_start, birth_year) VALUES ('vasily324', 'Oleg', 'C', 2001, 1971);
INSERT INTO drivers (user_login, name, category, career_start, birth_year) VALUES ('oleg324', 'Ivan', 'C', 2002, 1958);
INSERT INTO orders (datetime, driver_id, car_number, length, cargo_weight, cost) VALUES (20240303, 2, '8924 HP-3', 350.0, 400.0, 2500.0);
INSERT INTO orders (datetime, driver_id, car_number, length, cargo_weight, cost) VALUES (20240306, 2, '8924 HP-3', 456762.0, 100.0, 100.0);
INSERT INTO orders (datetime, driver_id, car_number, length, cargo_weight, cost) VALUES (20240408, 2, '8924 HP-3', 790.0, 120.0, 500.0);
