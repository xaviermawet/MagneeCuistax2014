create table PERSON
(
    id          INTEGER PRIMARY KEY AUTOINCREMENT,  -- identifiant unique sans signification particulière
    first_name  VARCHAR(80),                        -- Prénom de la personne
    last_name   VARCHAR(80),                        -- Nom de la personne
    age         INTEGER,                            -- Age de la personne

    ref_team    INTEGER,                            -- reférence de l'équipe (numéro du cuistax)

    -- Si on supprime l'équipe, la personne est supprimée aussi
    FOREIGN KEY (ref_team) REFERENCES TEAM(num_cuistax) ON DELETE CASCADE

    -- Si on supprime l'équipe, la personne n'est PAS supprimée
    -- FOREIGN KEY (ref_team) REFERENCES TEAM(num_cuistax)
);

create table TEAM
(
    num_cuistax INTEGER PRIMARY KEY,                -- Numéro du cuistax associé à l'équipe
    name        VARCHAR(80)                         -- Nom de l'équipe (unique)
);

create table RACE
(
    id          INTEGER PRIMARY KEY AUTOINCREMENT,  -- identifiant unique sans signification particulière
    name        VARCHAR(80),                        -- Nom identifiant la course
    date        DATETIME,                           -- Date à laquelle se déroule la course
    place       VARCHAR(80),                        -- Lieu où se déroule la course
    distance    FLOAT                               -- longueur en mètres du tracé de la course
);

create table LAP
(
    num         INTEGER,                            -- numéro du tour
    end_time    TIME,                               -- temps fin du tour

    ref_race    INTEGER,                            -- référence de la course
    ref_team    INTEGER,                            -- reférence de l'équipe (numéro du cuistax)

    FOREIGN KEY (ref_race) REFERENCES RACE(id) ON DELETE CASCADE,
    FOREIGN KEY (ref_team) REFERENCES TEAM(num_cuistax) ON DELETE CASCADE,
    PRIMARY KEY (num, ref_team, ref_race)
);
