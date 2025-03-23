#pragma once

#include <RE/Skyrim.h>

#include "FunctionSignatures.h"

namespace SkyrimScripting::Console {

    struct IConsoleManager {
        virtual bool start()                                                           = 0;
        virtual void stop()                                                            = 0;
        virtual bool running() const                                                   = 0;
        virtual bool run(const char* commandText, RE::TESObjectREFR* target = nullptr) = 0;
        virtual bool run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;
        virtual bool prepend_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;
        virtual bool add_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;
        virtual bool remove_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;
        virtual bool add_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) = 0;
        virtual bool remove_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        )                                                                       = 0;
        virtual bool prepend_console_handler(ConsoleHandlerFn consoleHandler)   = 0;
        virtual bool add_console_handler(ConsoleHandlerFn consoleHandler)       = 0;
        virtual bool remove_console_handler(ConsoleHandlerFn consoleHandler)    = 0;
        virtual bool add_console_listener(ConsoleListenerFn consoleListener)    = 0;
        virtual bool remove_console_listener(ConsoleListenerFn consoleListener) = 0;
        virtual bool claim_ownership(ConsoleHandlerFn)                          = 0;
        virtual bool release_ownership(ConsoleHandlerFn)                        = 0;

    protected:
        virtual ~IConsoleManager() = default;
    };
}
