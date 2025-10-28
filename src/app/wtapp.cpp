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
    std::cin >> v.title;

    std::cout << "Artist Count: ";
    std::cin >> num_artists;
    v.artists.resize(num_artists);

    for (size_t i = 0; i < num_artists; i++) {
        std::cout << "Artist " << i + 1 << ": ";
        std::cin >> input;

        // check for artist in cache or db
        // get ptr from cache or create new artist and get ptr
        // put in vector
    }

    std::cout << "Label: ";
    std::cin >> input;
    // check cache or db
    
    std::cout << "Source: ";
    std::cin >> input;
    // check cache or db


}