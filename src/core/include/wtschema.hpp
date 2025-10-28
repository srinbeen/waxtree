#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include <memory>

using tableid_t = int64_t;

struct Vinyl;
struct Track;
struct Work;
struct Artist;
struct Label;
struct Source;
struct Tag;

struct Vinyl {
    tableid_t id;

    std::string title;
    std::vector<std::shared_ptr<Artist>> artists;
    std::shared_ptr<Label> label;
    std::shared_ptr<Source> source;
    std::vector<std::shared_ptr<Tag>> tags;

    std::optional<std::string> notes;

    std::vector<std::shared_ptr<Track>> tracks;
};

struct Track {
    tableid_t id;

    std::shared_ptr<Work> work;
    
    std::string side;
    int idx;
    std::optional<std::string> duration;
    
    std::vector<std::shared_ptr<Artist>> features;
    
    std::weak_ptr<Vinyl> vinyl;
};

// vinyl-independent
struct Work {
    tableid_t id;

    std::string title;
    std::vector<std::shared_ptr<Artist>> artists;

    std::vector<std::weak_ptr<Track>> tracks;
};
struct Artist {
    tableid_t id;

    std::string name;
    std::string sort_name;

    std::vector<std::weak_ptr<Vinyl>> vinyls;
    std::vector<std::weak_ptr<Track>> track_features;
    std::vector<std::weak_ptr<Work>> works;
};

struct Label {
    tableid_t id;
    std::string name;

    std::vector<std::weak_ptr<Vinyl>> vinyls;
};
struct Source {
    tableid_t id;
    std::string name;

    std::vector<std::weak_ptr<Vinyl>> vinyls;
};
struct Tag {
    tableid_t id;
    std::string tag;

    std::vector<std::weak_ptr<Vinyl>> vinyls;
};