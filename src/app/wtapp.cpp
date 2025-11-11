#include <wtapp.hpp>
#include <iostream>
#include <tabulate/table.hpp>

WaxtreeApp::WaxtreeApp() : db{"waxtree.db3"} { }

void WaxtreeApp::run() {
    std::cout << "Waxtree...\n" << std::endl;

    while(true) {
        std::cout << options << std::endl;
        
        int input;
        std::cin >> input;
        WaxtreeApp::Options input_option = static_cast<WaxtreeApp::Options>(input);
        _handleInput(input_option);
    }
}

void WaxtreeApp::_handleInput(WaxtreeApp::Options option) {
    switch (option) {
        case WaxtreeApp::Options::VIEW:
            std::cout << "view not yet implemented..." << std::endl;
            break;
        case WaxtreeApp::Options::ADD:
            _constructIRAdd();
            break;
        default:
            break;
    }
}

void WaxtreeApp::_constructIRAdd() {
    Vinyl v;
    size_t num_artists;
    std::string input;

    std::cout << "Title: ";
    std::getline(std::cin, v.title);

    std::cout << "Artist Count: ";
    std::cin >> num_artists;
    v.artists.resize(num_artists);

    for (size_t i = 0; i < num_artists; i++) {
        std::cout << "Artist " << i + 1 << ": ";
        std::getline(std::cin, input);

        auto artists_with_input_name = db.getArtists(input);

        std::shared_ptr<Artist> selected_artist;
        switch (artists_with_input_name.size()) {
            case 0: {
                std::string sort_input;
                std::cout << "New Artist...\n Sort name: ";
                std::getline(std::cin, sort_input);

                v.artists[i] = db.insertArtist(input, sort_input);
                break;
            }
            case 1:
                v.artists[i] = artists_with_input_name[0];
                break;
            default: {
                int choice;
                for (size_t i = 0; i < artists_with_input_name.size(); i++) {
                    std::cout << i + 1 << ". id: " << artists_with_input_name[i]->id << std::endl;
                }

                std::cout << "Which artist: ";
                std::cin >> choice;

                v.artists[i] = artists_with_input_name[choice-1];
                break;
            }
        }
    }

    std::cout << "Label: ";
    std::getline(std::cin, input);
    v.label = db.getLabel(input);
    if (v.label == nullptr) v.label = db.insertLabel(input);
    
    std::cout << "Source: ";
    std::getline(std::cin, input);
    v.source = db.getSource(input);
    if (v.source == nullptr) v.source = db.insertSource(input);
    
    std::cout << "Notes: ";
    std::getline(std::cin, input);
    v.notes = input;
}