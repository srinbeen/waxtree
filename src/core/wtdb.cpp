#include <wtdb.hpp>
#include <iostream>

WaxtreeDB::WaxtreeDB(const std::string& s) : db{s, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE} {
    _createTables();
}

void WaxtreeDB::_createTables() {
    db.exec(R"(
        CREATE TABLE IF NOT EXISTS label(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            name        TEXT NOT NULL UNIQUE COLLATE NOCASE
        );
        
        CREATE TABLE IF NOT EXISTS source(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            name        TEXT NOT NULL UNIQUE COLLATE NOCASE
        );
        
        CREATE TABLE IF NOT EXISTS tag(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            value       TEXT NOT NULL UNIQUE COLLATE NOCASE
        );

        CREATE TABLE IF NOT EXISTS vinyl(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            title       TEXT NOT NULL,
            label_id    INTEGER,
            source_id   INTEGER,
            notes       TEXT,

            FOREIGN KEY(label_id)   REFERENCES label(id),
            FOREIGN KEY(source_id)  REFERENCES source(id)
        );

        CREATE TABLE IF NOT EXISTS vinyltag(
            vinyl_id    INTEGER NOT NULL,
            tag_id      INTEGER NOT NULL,

            FOREIGN KEY(vinyl_id)   REFERENCES vinyl(id)
                ON DELETE CASCADE,
            FOREIGN KEY(tag_id)     REFERENCES tag(id)
                ON DELETE CASCADE,

            PRIMARY KEY(vinyl_id, tag_id)
        );
        
        CREATE TABLE IF NOT EXISTS artist(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            name        TEXT NOT NULL UNIQUE COLLATE NOCASE,
            sort_name   TEXT NOT NULL COLLATE NOCASE
        );

        CREATE TABLE IF NOT EXISTS vinylartist(
            vinyl_id    INTEGER NOT NULL,
            artist_id   INTEGER NOT NULL,

            FOREIGN KEY(vinyl_id)   REFERENCES vinyl(id)
                ON DELETE CASCADE,
            FOREIGN KEY(artist_id)  REFERENCES artist(id)
                ON DELETE CASCADE,

            PRIMARY KEY(vinyl_id, artist_id)
        );
        
        CREATE TABLE IF NOT EXISTS work(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            title       TEXT NOT NULL COLLATE NOCASE
        );

        CREATE TABLE IF NOT EXISTS workartist(
            work_id     INTEGER NOT NULL,
            artist_id   INTEGER NOT NULL,

            FOREIGN KEY(work_id)    REFERENCES work(id)
                ON DELETE CASCADE,
            FOREIGN KEY(artist_id)  REFERENCES artist(id)
                ON DELETE CASCADE,

            PRIMARY KEY(work_id, artist_id)
        );

        CREATE TABLE IF NOT EXISTS track(
            id          INTEGER PRIMARY KEY AUTOINCREMENT,
            work_id     INTEGER NOT NULL,
            vinyl_id    INTEGER NOT NULL,
            side        TEXT NOT NULL,
            idx         INTEGER NOT NULL,
            duration    TEXT,

            FOREIGN KEY(work_id)    REFERENCES work(id),
            FOREIGN KEY(vinyl_id)   REFERENCES vinyl(id)
                ON DELETE CASCADE
        );

        CREATE TABLE IF NOT EXISTS trackfeature(
            track_id    INTEGER NOT NULL,
            feature_id  INTEGER NOT NULL,

            FOREIGN KEY(track_id)    REFERENCES track(id)
                ON DELETE CASCADE,
            FOREIGN KEY(feature_id)  REFERENCES artist(id)
                ON DELETE CASCADE,

            PRIMARY KEY(track_id, feature_id)
        );
    )");
}