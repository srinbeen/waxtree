#include <wtdb.hpp>
#include <iostream>

WaxtreeDB::WaxtreeDB(const std::string& s) : db{s, SQLite::OPEN_READWRITE|SQLite::OPEN_CREATE} {
    _createTables();
}

void WaxtreeDB::_createTables() {
    SQLite::Transaction transaction(db);
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
            name        TEXT NOT NULL COLLATE NOCASE,
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
    transaction.commit();
}

std::vector<std::shared_ptr<Artist>> WaxtreeDB::getArtists(const std::string& name) {
    SQLite::Statement q{db, "SELECT * FROM artist WHERE name = ?"};
    q.bind(1, name);

    std::vector<std::shared_ptr<Artist>> artists;
    tableid_t id = 0;
    while (q.executeStep()) {
        id = 0;

        id = q.getColumn("id").getInt64();
        auto it = artist_cache.find(id);

        // not in cache but in db
        if (it == artist_cache.end()) {
            std::shared_ptr<Artist> artist = std::make_shared<Artist>(
                id,
                q.getColumn("name").getText(),
                q.getColumn("sort_name").getText()
            );

            artist_cache[id] = artist;
            artists.push_back(artist);
        }
        // in cache
        else {
            artists.push_back(it->second);
        }
    }

    return artists;
}

std::shared_ptr<Label> WaxtreeDB::getLabel(const std::string& name) {
    SQLite::Statement q{db, "SELECT * FROM label WHERE name = ?"};
    q.bind(1, name);

    std::shared_ptr<Label> label = nullptr;
    tableid_t id = 0;
    while (q.executeStep()) {
        id = 0;

        id = q.getColumn("id").getInt64();
        auto it = label_cache.find(id);

        // not in cache but in db
        if (it == label_cache.end()) {
            label = std::make_shared<Label>(
                id,
                q.getColumn("name").getText()
            );

            label_cache[id] = label;
        }
        // in cache
        else {
            label = it->second;
        }
    }

    return label;
}

// todo: make it a template?
std::shared_ptr<Source> WaxtreeDB::getSource(const std::string& name) {
    SQLite::Statement q{db, "SELECT * FROM source WHERE name = ?"};
    q.bind(1, name);

    std::shared_ptr<Source> source = nullptr;
    tableid_t id = 0;
    while (q.executeStep()) {
        id = 0;

        id = q.getColumn("id").getInt64();
        auto it = source_cache.find(id);

        // not in cache but in db
        if (it == source_cache.end()) {
            source = std::make_shared<Source>(
                id,
                q.getColumn("name").getText()
            );

            source_cache[id] = source;
        }
        // in cache
        else {
            source = it->second;
        }
    }

    return source;
}

// TODO: error-handling
std::shared_ptr<Artist> WaxtreeDB::insertArtist(const std::string& name, const std::string& sort_name) {
    SQLite::Statement q{db, "INSERT INTO artist (name, sort_name) VALUES (?, ?)"};
    q.bind(1, name);
    q.bind(2, sort_name);

    std::shared_ptr<Artist> artist = nullptr;
    if (q.exec() > 0) {
        tableid_t id = db.getLastInsertRowid();
        artist = std::make_shared<Artist>(id, name, sort_name);
        artist_cache[id] = artist;
    }

    return artist;
}

std::shared_ptr<Label> WaxtreeDB::insertLabel(const std::string& name) {
    SQLite::Statement q{db, "INSERT INTO label (name) VALUES (?)"};
    q.bind(1, name);

    std::shared_ptr<Label> label = nullptr;
    if (q.exec() > 0) {
        tableid_t id = db.getLastInsertRowid();
        label = std::make_shared<Label>(id, name);
        label_cache[id] = label;
    }

    return label;
}

std::shared_ptr<Source> WaxtreeDB::insertSource(const std::string& name) {
    SQLite::Statement q{db, "INSERT INTO source (name) VALUES (?)"};
    q.bind(1, name);

    std::shared_ptr<Source> source = nullptr;
    if (q.exec() > 0) {
        tableid_t id = db.getLastInsertRowid();
        source = std::make_shared<Source>(id, name);
        source_cache[id] = source;
    }

    return source;
}

template <typename T> std::shared_ptr<T> WaxtreeDB::getModel(const T& model) {
    SQLite::Statement q{db, "SELECT * FROM " + T.model_name + "WHERE name = ?"};
    q.bind(1, name);

    std::shared_ptr<Source> source = nullptr;
    tableid_t id = 0;
    while (q.executeStep()) {
        id = 0;

        id = q.getColumn("id").getInt64();
        auto it = source_cache.find(id);

        // not in cache but in db
        if (it == source_cache.end()) {
            source = std::make_shared<Source>(
                id,
                q.getColumn("name").getText()
            );

            source_cache[id] = source;
        }
        // in cache
        else {
            source = it->second;
        }
    }

    return source;
}