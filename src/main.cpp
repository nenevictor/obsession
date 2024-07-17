#include "OApp.hpp"

int main(int _argc, char const *_argv[])
{
    UNUSED(_argc);
    UNUSED(_argv);

    i32_t result = 0;
    OApp app;

    d_clogf("%s", CLOG_INFO "App initialization...");
    result = app.init();
    if (result)
    {
        d_clogf("%s%i", CLOG_ERR "App failed initialization; Code: ", result);
        return EXIT_FAILURE;
    };

    d_clogf("%s", CLOG_INFO "App initialization successfully passed.");

    d_clogf("%s", CLOG_INFO "App running...");
    result = app.run();

    if (result)
    {
        d_clogf("%s%i", CLOG_ERR "App error during \"run\" function; Code: ", result);
        return EXIT_FAILURE;
    };

    d_clogf("%s", CLOG_INFO "App successfully exited.");

    return EXIT_SUCCESS;
};