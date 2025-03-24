#include <SkyrimScripting/Plugin.h>

#include "ConsoleManager.h"
#include "Hook.h"

bool ConsoleCommandHandler(std::string_view commandText, RE::TESObjectREFR* target) {
    return SkyrimScripting::Console::ConsoleManager::instance()->run(commandText.data(), target);
}

SKSEPlugin_OnInputLoaded {
    Hook::set_command_handler(ConsoleCommandHandler);
    Hook::install();
}
