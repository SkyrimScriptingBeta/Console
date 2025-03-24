#pragma once

#include <SkyrimScripting/Console/IConsoleManager.h>
#include <collections.h>

namespace SkyrimScripting::Console {
    class ConsoleManager : public IConsoleManager {
        std::atomic<bool>                                      _enabled = false;
        std::vector<ConsoleHandlerFn>                          _ownershipHandlersStack;
        std::vector<ConsoleListenerFn>                         _consoleListeners;
        std::vector<ConsoleHandlerFn>                          _consoleHandlers;
        std::vector<ConsoleHandlerFn>                          _priorityConsoleHandlers;
        collections_map<std::string, ConsoleCommandHandlerFn>  _commandHandlers;
        collections_map<std::string, ConsoleCommandListenerFn> _commandListeners;

    public:
        ~ConsoleManager() override = default;

        bool               enable() override;
        void               disable() override;
        bool               enabled() const override;
        RE::TESObjectREFR* selected_ref() override;
        void               select_ref(RE::TESObjectREFR* reference) override;
        void               log(const char* message) override;
        const char*        last_output() override;
        bool run_native(const char* commandText, RE::TESObjectREFR* target = nullptr) override;
        bool run(
            const char* commandText, RE::TESObjectREFR* target = nullptr,
            bool ignoreConsoleOwnership = true
        ) override;
        void run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override;

        ConsoleCommandHandlerFn prepend_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) override;
        ConsoleCommandHandlerFn add_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) override;
        bool remove_command_handler(const char* commandName, ConsoleCommandHandlerFn commandHandler)
            override;
        void clear_command_handlers(const char* commandName) override;
        void run_command_handlers(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override;

        ConsoleCommandListenerFn add_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) override;
        ConsoleCommandListenerFn remove_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) override;
        void clear_command_listeners() override;
        void run_command_listeners(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override;

        ConsoleHandlerFn prepend_priority_console_handler(ConsoleHandlerFn consoleHandler) override;
        ConsoleHandlerFn add_priority_console_handler(ConsoleHandlerFn consoleHandler) override;
        ConsoleHandlerFn remove_priority_console_handler(ConsoleHandlerFn consoleHandler) override;
        void             clear_priority_console_handlers() override;
        void             run_priority_console_handlers(
                        const char* commandText, RE::TESObjectREFR* target = nullptr
                    ) override;

        ConsoleHandlerFn prepend_console_handler(ConsoleHandlerFn consoleHandler) override;
        ConsoleHandlerFn add_console_handler(ConsoleHandlerFn consoleHandler) override;
        ConsoleHandlerFn remove_console_handler(ConsoleHandlerFn consoleHandler) override;
        void             clear_console_handlers() override;
        void run_console_handlers(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override;

        ConsoleListenerFn add_console_listener(ConsoleListenerFn consoleListener) override;
        ConsoleListenerFn remove_console_listener(ConsoleListenerFn consoleListener) override;
        void              clear_console_listeners() override;
        void run_console_listeners(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override;

        bool claim_ownership(ConsoleHandlerFn consoleHandler) override;
        bool release_ownership(ConsoleHandlerFn = nullptr) override;
        void clear_ownership_handlers() override;
        bool is_owned() const override;
        bool run_owning_handler(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override;
    };
}
