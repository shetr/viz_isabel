#include "viz_app.h"
#include "test_app.h"

int main(int argc, char** argv) {
    App app(new VizApp());
    app.Run();
    return 0;
}