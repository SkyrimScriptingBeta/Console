#include <SkyrimScripting/Plugin.h>

#include "ConsoleManager.h"
#include "Hook.h"

auto* consoleManager = SkyrimScripting::Console::ConsoleManager::instance();

bool ConsoleCommandHandler(std::string_view commandText, RE::TESObjectREFR* target) {
    return consoleManager->run(commandText.data(), target);
}

auto testCommandHandler = function_pointer([](const char* commandText, RE::TESObjectREFR* target) {
    // Does commandText start with "test"?
    if (strncmp(commandText, "test", 4) != 0) return false;
    ConsoleLog("Test command detected!");
    return true;
});

SKSEPlugin_OnInputLoaded {
    consoleManager->add_console_handler(&testCommandHandler);
    Hook::set_command_handler(ConsoleCommandHandler);
    Hook::install();
}
