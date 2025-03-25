#pragma once

#include <SkyrimScripting/Console/IConsoleManager.h>  // IWYU pragma: export

namespace SkyrimScripting::Console {
    class ConsoleManager : public IConsoleManager {
        IConsoleManager* _consoleManager;

    public:
        ConsoleManager(IConsoleManager* consoleManager) : _consoleManager(consoleManager) {}

        static IConsoleManager* instance() {
            // ...implementation...
        }

        bool enable() override { return _consoleManager->enable(); }

        void disable() override { _consoleManager->disable(); }

        bool enabled() const override { return _consoleManager->enabled(); }

        RE::TESObjectREFR* selected_ref() override { return _consoleManager->selected_ref(); }

        void select_ref(RE::TESObjectREFR* reference) override {
            _consoleManager->select_ref(reference);
        }

        void log(const char* message) override { _consoleManager->log(message); }

        const char* last_output() override { return _consoleManager->last_output(); }

        bool run_native(const char* commandText, RE::TESObjectREFR* target = nullptr) override {
            return _consoleManager->run_native(commandText, target);
        }

        bool run(
            const char* commandText, RE::TESObjectREFR* target, bool ignoreConsoleOwnership = false
        ) override {
            return _consoleManager->run(commandText, target, ignoreConsoleOwnership);
        }

        void run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            _consoleManager->run_command(commandName, commandText, target);
        }

        ConsoleCommandHandlerFn prepend_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) override {
            return _consoleManager->prepend_command_handler(commandName, commandHandler);
        }

        ConsoleCommandHandlerFn add_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) override {
            return _consoleManager->add_command_handler(commandName, commandHandler);
        }

        bool remove_command_handler(const char* commandName, ConsoleCommandHandlerFn commandHandler)
            override {
            return _consoleManager->remove_command_handler(commandName, commandHandler);
        }

        void clear_command_handlers(const char* commandName) override {
            _consoleManager->clear_command_handlers(commandName);
        }

        void run_command_handlers(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            _consoleManager->run_command_handlers(commandName, commandText, target);
        }

        ConsoleCommandListenerFn add_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) override {
            return _consoleManager->add_command_listener(commandName, commandListener);
        }

        ConsoleCommandListenerFn remove_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) override {
            return _consoleManager->remove_command_listener(commandName, commandListener);
        }

        void clear_command_listeners() override { _consoleManager->clear_command_listeners(); }

        void run_command_listeners(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            _consoleManager->run_command_listeners(commandName, commandText, target);
        }

        ConsoleHandlerFn prepend_priority_console_handler(ConsoleHandlerFn consoleHandler
        ) override {
            return _consoleManager->prepend_priority_console_handler(consoleHandler);
        }

        ConsoleHandlerFn add_priority_console_handler(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->add_priority_console_handler(consoleHandler);
        }

        ConsoleHandlerFn remove_priority_console_handler(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->remove_priority_console_handler(consoleHandler);
        }

        void clear_priority_console_handlers() override {
            _consoleManager->clear_priority_console_handlers();
        }

        void run_priority_console_handlers(
            const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            _consoleManager->run_priority_console_handlers(commandText, target);
        }

        ConsoleHandlerFn prepend_console_handler(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->prepend_console_handler(consoleHandler);
        }

        ConsoleHandlerFn add_console_handler(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->add_console_handler(consoleHandler);
        }

        ConsoleHandlerFn remove_console_handler(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->remove_console_handler(consoleHandler);
        }

        void clear_console_handlers() override { _consoleManager->clear_console_handlers(); }

        bool run_console_handlers(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override {
            return _consoleManager->run_console_handlers(commandText, target);
        }

        ConsoleListenerFn add_console_listener(ConsoleListenerFn consoleListener) override {
            return _consoleManager->add_console_listener(consoleListener);
        }

        ConsoleListenerFn remove_console_listener(ConsoleListenerFn consoleListener) override {
            return _consoleManager->remove_console_listener(consoleListener);
        }

        void clear_console_listeners() override { _consoleManager->clear_console_listeners(); }

        void run_console_listeners(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override {
            _consoleManager->run_console_listeners(commandText, target);
        }

        bool claim_ownership(ConsoleHandlerFn consoleHandler) override {
            return _consoleManager->claim_ownership(consoleHandler);
        }

        bool release_ownership(ConsoleHandlerFn consoleHandler = nullptr) override {
            return _consoleManager->release_ownership(consoleHandler);
        }

        void clear_ownership_handlers() override { _consoleManager->clear_ownership_handlers(); }

        bool is_owned() const override { return _consoleManager->is_owned(); }

        bool run_owning_handler(const char* commandText, RE::TESObjectREFR* target = nullptr)
            override {
            return _consoleManager->run_owning_handler(commandText, target);
        }

        /*
         * std::function variations!
         */

        using StdConsoleHandlerFn =
            std::function<bool(const char* commandText, RE::TESObjectREFR* target)>*;

        using StdConsoleListenerFn =
            std::function<void(const char* commandText, RE::TESObjectREFR* target)>*;

        using StdConsoleCommandHandlerFn = std::function<
            bool(const char* command, const char* commandText, RE::TESObjectREFR* target)>*;

        using StdConsoleCommandListenerFn = std::function<
            void(const char* command, const char* commandText, RE::TESObjectREFR* target)>*;

        // Duplicate definitions for std::function variations

        ConsoleCommandHandlerFn prepend_command_handler(
            const char* commandName, StdConsoleCommandHandlerFn commandHandler
        ) {
            return prepend_command_handler(
                commandName, reinterpret_cast<ConsoleCommandHandlerFn>(commandHandler)
            );
        }

        ConsoleCommandHandlerFn add_command_handler(
            const char* commandName, StdConsoleCommandHandlerFn commandHandler
        ) {
            return add_command_handler(
                commandName, reinterpret_cast<ConsoleCommandHandlerFn>(commandHandler)
            );
        }

        bool remove_command_handler(
            const char* commandName, StdConsoleCommandHandlerFn commandHandler
        ) {
            return remove_command_handler(
                commandName, reinterpret_cast<ConsoleCommandHandlerFn>(commandHandler)
            );
        }

        ConsoleCommandListenerFn add_command_listener(
            const char* commandName, StdConsoleCommandListenerFn commandListener
        ) {
            return add_command_listener(
                commandName, reinterpret_cast<ConsoleCommandListenerFn>(commandListener)
            );
        }

        ConsoleCommandListenerFn remove_command_listener(
            const char* commandName, StdConsoleCommandListenerFn commandListener
        ) {
            return remove_command_listener(
                commandName, reinterpret_cast<ConsoleCommandListenerFn>(commandListener)
            );
        }

        ConsoleHandlerFn prepend_priority_console_handler(StdConsoleHandlerFn consoleHandler) {
            return prepend_priority_console_handler(
                reinterpret_cast<ConsoleHandlerFn>(consoleHandler)
            );
        }

        ConsoleHandlerFn add_priority_console_handler(StdConsoleHandlerFn consoleHandler) {
            return add_priority_console_handler(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }

        ConsoleHandlerFn remove_priority_console_handler(StdConsoleHandlerFn consoleHandler) {
            return remove_priority_console_handler(reinterpret_cast<ConsoleHandlerFn>(consoleHandler
            ));
        }

        ConsoleHandlerFn prepend_console_handler(StdConsoleHandlerFn consoleHandler) {
            return prepend_console_handler(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }

        ConsoleHandlerFn add_console_handler(StdConsoleHandlerFn consoleHandler) {
            return add_console_handler(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }

        ConsoleHandlerFn remove_console_handler(StdConsoleHandlerFn consoleHandler) {
            return remove_console_handler(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }

        ConsoleListenerFn add_console_listener(StdConsoleListenerFn consoleListener) {
            return add_console_listener(reinterpret_cast<ConsoleListenerFn>(consoleListener));
        }

        ConsoleListenerFn remove_console_listener(StdConsoleListenerFn consoleListener) {
            return remove_console_listener(reinterpret_cast<ConsoleListenerFn>(consoleListener));
        }

        bool claim_ownership(StdConsoleHandlerFn consoleHandler) {
            return claim_ownership(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }

        bool release_ownership(StdConsoleHandlerFn consoleHandler = nullptr) {
            return release_ownership(reinterpret_cast<ConsoleHandlerFn>(consoleHandler));
        }
    };
}
