#include <SkyrimScripting/Plugin.h>
#include <SkyrimScripting/Services.h>

#include "ConsoleManagerService.h"
#include "Hook.h"

auto* consoleManagerService = SkyrimScripting::Console::ConsoleManagerService::instance();

bool ConsoleCommandHandler(std::string_view commandText, RE::TESObjectREFR* target) {
    return consoleManagerService->run(commandText.data(), target);
}

SKSEPlugin_Entrypoint {
    Hook::set_command_handler(ConsoleCommandHandler);
    Hook::install();
    SkyrimScripting::Services::Initialize();
}

SKSEPlugin_OnPostLoad {
    RegisterService(
        consoleManagerService, "ConsoleManager", "A service provided by SkyrimScripting.Console."
    );
}
