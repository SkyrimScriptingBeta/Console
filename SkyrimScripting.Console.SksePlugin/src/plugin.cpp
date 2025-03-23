#include <SkyrimScripting/Plugin.h>

#include <string_view>

#include "Hook.h"

bool ConsoleCommandHandler(std::string_view commandText, RE::TESObjectREFR* target) {
    SKSE::log::info("HELLO FROM OVERRIDEN CONSOLE COMMAND!");
    SKSE::log::info("The Command: {}", commandText);

    // for (auto& callback : CommandRegistration::CommandCallbacks)
    //     if (callback(commandText)) return true;

    return false;
}

SKSEPlugin_OnInputLoaded {
    Hook::set_command_handler(ConsoleCommandHandler);
    Hook::install();
}
