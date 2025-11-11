#pragma once

#include <string>
#include <unordered_map>

#include <wtschema.hpp>

#include <SQLiteCpp/SQLiteCpp.h>

class WaxtreeDB {
public:
    WaxtreeDB() = delete;
    WaxtreeDB(const std::string& s);

    std::vector<std::shared_ptr<Artist>> getArtists(const std::string& name);
    std::shared_ptr<Artist> insertArtist(const std::string& name, const std::string& sort_name);
    
    std::shared_ptr<Label> getLabel(const std::string& name);
    std::shared_ptr<Label> insertLabel(const std::string& name);
    
    std::shared_ptr<Source> getSource(const std::string& name);
    std::shared_ptr<Source> insertSource(const std::string& name);

    template <typename T> std::shared_ptr<T> getModel(const T& model);
    template <typename T> std::vector<std::shared_ptr<T>> getModels(const T& model);
    template <typename T> std::shared_ptr<T> insertModel(const T& model);
    
    // void addVinyl(const Vinyl& vinyl);

    

private:
// #ifdef USE_SQLITECPP
    SQLite::Database db;
    // caches
    std::map<tableid_t, std::shared_ptr<Artist>> artist_cache; 
    std::map<tableid_t, std::shared_ptr<Label>> label_cache; 
    std::map<tableid_t, std::shared_ptr<Source>> source_cache; 

    void _createTables();
// #endif
};