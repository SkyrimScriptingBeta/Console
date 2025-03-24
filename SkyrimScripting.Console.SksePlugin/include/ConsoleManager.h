#pragma once

#include <SkyrimScripting/Console/IConsoleManager.h>
#include <collections.h>

namespace SkyrimScripting::Console {
    class ConsoleManager : IConsoleManager {
        std::atomic<bool>                                      _enabled = false;
        std::vector<ConsoleHandlerFn>                          _ownershipHandlersStack;
        std::vector<ConsoleListenerFn>                         _consoleListeners;
        std::vector<ConsoleHandlerFn>                          _consoleHandlers;
        collections_map<std::string, ConsoleCommandHandlerFn>  _commandHandlers;
        collections_map<std::string, ConsoleCommandListenerFn> _commandListeners;

    public:
        bool enable() override { return !_enabled.exchange(true); }
        void disable() override { _enabled = false; }
        bool enabled() const override { return _enabled; }

        void run(
            const char* commandText, RE::TESObjectREFR* target, bool ignoreConsoleOwnership = true
        ) override {
            // 1. Runs all console listeners.
            run_console_listeners(commandText, target);

            // 2. Runs all priority console handlers. If any returns true, the command is considered
            // handled.
            run_console_handlers(commandText);

            // 3. Determines the command name from the first part of the command text.
            std::string commandName = commandText;
            size_t      spaceIndex  = commandName.find(' ');
            if (spaceIndex != std::string::npos) {
                commandName = commandName.substr(0, spaceIndex);
            }

            // 4. Runs all command listeners for the command name.
            run_command_listeners(commandName.c_str(), commandText);

            // 5. Runs all command handlers for the command name. If any returns true, the command
            // is considered handled.
            run_command_handlers(commandName.c_str(), commandText);

            // 6. Runs all command listeners for the command name.
            run_console_handlers(commandText);
        }

        void run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            // 1. Runs all command listeners for the command name.
            run_command_listeners(commandName, commandText, target);

            // 2. Runs all command handlers for the command name. If any returns true, the command
            // is considered handled.
            run_command_handlers(commandName, commandText, target);
        }
    };
}
