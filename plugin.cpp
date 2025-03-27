#include <SkyrimScripting/Console.h>
#include <SkyrimScripting/Plugin.h>

using namespace SkyrimScripting::Console;

auto consoleHandler = function_pointer([](const char* commandText, RE::TESObjectREFR* target) {
    Log("ConsoleCommandHandler called with commandText: {}", commandText);
    return false;
});

SKSEPlugin_Entrypoint { SkyrimScripting::Console::Initialize(); }

SKSEPlugin_OnPostPostLoad {
    if (auto* consoleManager = GetConsoleManager()) {
        Log("!! ConsoleManager service found.");
        consoleManager->add_console_handler(&consoleHandler);
    } else {
        Log("!! ConsoleManager service not found!");
    }
}
