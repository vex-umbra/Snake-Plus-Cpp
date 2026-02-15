#include "app/App.h"
#include <iostream>

int main(int, char**) {
    App app;
    if (!app.init()) {
        std::cerr << "Failed to init app.\n";
        return 1;
    }
    app.run();
    return 0;
}
