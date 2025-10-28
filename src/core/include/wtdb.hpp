#pragma once

#include <string>

#include <wtschema.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

class WaxtreeDB {
public:
    WaxtreeDB() = delete;
    WaxtreeDB(const std::string& s);

    void addVinyl(const Vinyl& vinyl);

    

private:
// #ifdef USE_SQLITECPP
    SQLite::Database db;

    void _createTables();
// #endif
};