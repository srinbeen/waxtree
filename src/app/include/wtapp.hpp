#pragma once
#include <wtdb.hpp>

class WaxtreeApp {
public:
    enum class Options {
        VIEW = 1,
        ADD
    };

    WaxtreeApp();
    void run();

private:
    WaxtreeDB db;
    const char* options = R"(
Actions:
    1.  view all vinyls
    2.  add new vinyl
)";

    void _handleInput(WaxtreeApp::Options option);
    void _constructIRAdd();
};