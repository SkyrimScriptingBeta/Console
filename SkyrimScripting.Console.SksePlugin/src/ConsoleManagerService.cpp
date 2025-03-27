#include "ConsoleManagerService.h"

#include <SKSE/Logger.h>  // Include SKSE logging

#include <string>

namespace SkyrimScripting::Console {

    IConsoleManagerService* ConsoleManagerService::instance() {
        static ConsoleManagerService instance;
        SKSE::log::trace("instance() called");
        return &instance;
    }

    bool ConsoleManagerService::enable() {
        SKSE::log::info("enable() called");
        _enabled = true;
        return true;
    }

    void ConsoleManagerService::disable() {
        SKSE::log::info("disable() called");
        _enabled = false;
    }

    bool ConsoleManagerService::enabled() const {
        SKSE::log::trace("enabled() called");
        return _enabled;
    }

    /**
     * `selected_ref`, `select_ref`, and `run_native` functions source come from ConsoleUtilSSE
     * - https://github.com/VersuchDrei/ConsoleUtilSSE
     * - License: MIT
     *
     * _Provided here on ConsoleManager for convenience_
     */
    RE::TESObjectREFR* ConsoleManagerService::selected_ref() {
        SKSE::log::debug("selected_ref() called");
        const auto selectedRef = RE::Console::GetSelectedRef();
        return selectedRef.get();
    }

    /**
     * `selected_ref`, `select_ref`, and `run_native` functions source come from ConsoleUtilSSE
     * - https://github.com/VersuchDrei/ConsoleUtilSSE
     * - License: MIT
     *
     * _Provided here on ConsoleManager for convenience_
     */
    void ConsoleManagerService::select_ref(RE::TESObjectREFR* reference) {
        SKSE::log::info("select_ref() called with reference: {}", reference ? "valid" : "nullptr");
        if (reference) {
            const auto factory          = RE::MessageDataFactoryManager::GetSingleton();
            const auto interfaceStrings = RE::InterfaceStrings::GetSingleton();
            const auto creator =
                factory && interfaceStrings
                    ? factory->GetCreator<RE::ConsoleData>(interfaceStrings->consoleData)
                    : nullptr;

            const auto consoleData  = creator ? creator->Create() : nullptr;
            const auto messageQueue = RE::UIMessageQueue::GetSingleton();
            if (consoleData && messageQueue) {
                consoleData->type    = static_cast<RE::ConsoleData::DataType>(1);
                consoleData->pickRef = reference->CreateRefHandle();
                messageQueue->AddMessage(
                    interfaceStrings->console, RE::UI_MESSAGE_TYPE::kUpdate, consoleData
                );
            }
        } else {
            const auto ui      = RE::UI::GetSingleton();
            const auto console = ui ? ui->GetMenu<RE::Console>() : nullptr;
            if (console) {
                const RE::ObjectRefHandle nothing;
                console->SetSelectedRef(nothing);
            }
        }
    }

    void ConsoleManagerService::log(const char* message) {
        SKSE::log::debug("log() called with message: {}", message ? message : "nullptr");
        if (!message) return;
        auto console = RE::ConsoleLog::GetSingleton();
        if (console) console->Print(message);
    }

    const char* ConsoleManagerService::last_output() {
        SKSE::log::trace("last_output() called");
        auto console = RE::ConsoleLog::GetSingleton();
        if (console) return console->lastMessage;
        return "";
    }

    /**
     * `selected_ref`, `select_ref`, and `run_native` functions source come from ConsoleUtilSSE
     * - https://github.com/VersuchDrei/ConsoleUtilSSE
     * - License: MIT
     *
     * _Provided here on ConsoleManager for convenience_
     */
    bool ConsoleManagerService::run_native(const char* commandText, RE::TESObjectREFR* target) {
        SKSE::log::info(
            "run_native() called with commandText: {} and target: {}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr"
        );
        if (!commandText) return false;
        const auto scriptFactory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
        const auto script        = scriptFactory ? scriptFactory->Create() : nullptr;
        if (script) {
            script->SetCommand(commandText);
            script->CompileAndRun(target == nullptr ? selected_ref() : target);
            delete script;
            return true;
        }
        return false;
    }

    bool ConsoleManagerService::run(
        const char* commandText, RE::TESObjectREFR* target, bool ignoreConsoleOwnership
    ) {
        SKSE::log::info(
            "run() called with commandText: {}, target: {}, "
            "ignoreConsoleOwnership: {}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr",
            ignoreConsoleOwnership
        );
        if (!target) target = selected_ref();

        // Run console listeners first
        run_console_listeners(commandText, target);

        // Check ownership
        if (!ignoreConsoleOwnership && is_owned()) {
            return run_owning_handler(commandText, target);
        }

        // Run priority console handlers
        for (auto& handler : _priorityConsoleHandlers) {
            if (handler->invoke(commandText, target)) return true;
        }

        // Parse command name
        std::string cmdText(commandText);
        std::string commandName;
        size_t      spacePos = cmdText.find(' ');
        if (spacePos != std::string::npos) {
            commandName = cmdText.substr(0, spacePos);
        } else {
            commandName = cmdText;
        }

        // Run command listeners
        run_command_listeners(commandName.c_str(), commandText, target);

        // Run command handlers
        auto cmdHandlerIt = _commandHandlers.find(commandName);
        if (cmdHandlerIt != _commandHandlers.end()) {
            if (cmdHandlerIt->second->invoke(commandName.c_str(), commandText, target)) return true;
        }

        // Run regular console handlers
        if (run_console_handlers(commandText, target)) return true;

        return false;
    }

    void ConsoleManagerService::run_command(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_command() called with commandName: {}, commandText: {}, "
            "target: {}",
            commandName ? commandName : "nullptr", commandText ? commandText : "nullptr",
            target ? "valid" : "nullptr"
        );
        if (!target) target = selected_ref();

        // Run command listeners
        run_command_listeners(commandName, commandText, target);

        // Run command handlers
        run_command_handlers(commandName, commandText, target);
    }

    ConsoleCommandHandlerFn ConsoleManagerService::prepend_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        SKSE::log::info(
            "prepend_command_handler() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        _commandHandlers.insert_or_assign(std::string(commandName), commandHandler);
        return commandHandler;
    }

    ConsoleCommandHandlerFn ConsoleManagerService::add_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        SKSE::log::info(
            "add_command_handler() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        _commandHandlers.insert_or_assign(std::string(commandName), commandHandler);
        return commandHandler;
    }

    bool ConsoleManagerService::remove_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        SKSE::log::warn(
            "remove_command_handler() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        auto it = _commandHandlers.find(commandName);
        if (it != _commandHandlers.end() && it->second == commandHandler) {
            _commandHandlers.erase(it);
            return true;
        }
        return false;
    }

    void ConsoleManagerService::clear_command_handlers(const char* commandName) {
        SKSE::log::warn(
            "clear_command_handlers() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        _commandHandlers.erase(commandName);
    }

    void ConsoleManagerService::run_command_handlers(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_command_handlers() called with commandName: {}, "
            "commandText: {}, target: {}",
            commandName ? commandName : "nullptr", commandText ? commandText : "nullptr",
            target ? "valid" : "nullptr"
        );
        auto it = _commandHandlers.find(commandName);
        if (it != _commandHandlers.end()) {
            it->second->invoke(commandName, commandText, target);
        }
    }

    ConsoleCommandListenerFn ConsoleManagerService::add_command_listener(
        const char* commandName, ConsoleCommandListenerFn commandListener
    ) {
        SKSE::log::info(
            "add_command_listener() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        _commandListeners.insert_or_assign(std::string(commandName), commandListener);
        return commandListener;
    }

    ConsoleCommandListenerFn ConsoleManagerService::remove_command_listener(
        const char* commandName, ConsoleCommandListenerFn commandListener
    ) {
        SKSE::log::warn(
            "remove_command_listener() called with commandName: {}",
            commandName ? commandName : "nullptr"
        );
        auto it = _commandListeners.find(commandName);
        if (it != _commandListeners.end() && it->second == commandListener) {
            _commandListeners.erase(it);
            return commandListener;
        }
        return nullptr;
    }

    void ConsoleManagerService::clear_command_listeners() {
        SKSE::log::warn("clear_command_listeners() called");
        _commandListeners.clear();
    }

    void ConsoleManagerService::run_command_listeners(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_command_listeners() called with commandName: {}, "
            "commandText: {}, target: {}",
            commandName ? commandName : "nullptr", commandText ? commandText : "nullptr",
            target ? "valid" : "nullptr"
        );
        auto it = _commandListeners.find(commandName);
        if (it != _commandListeners.end()) {
            it->second->invoke(commandName, commandText, target);
        }
    }

    ConsoleHandlerFn ConsoleManagerService::prepend_priority_console_handler(
        ConsoleHandlerFn consoleHandler
    ) {
        SKSE::log::info("prepend_priority_console_handler() called");
        _priorityConsoleHandlers.insert(_priorityConsoleHandlers.begin(), consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManagerService::add_priority_console_handler(
        ConsoleHandlerFn consoleHandler
    ) {
        SKSE::log::info("add_priority_console_handler() called");
        _priorityConsoleHandlers.push_back(consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManagerService::remove_priority_console_handler(
        ConsoleHandlerFn consoleHandler
    ) {
        SKSE::log::warn("remove_priority_console_handler() called");
        auto it = std::find(
            _priorityConsoleHandlers.begin(), _priorityConsoleHandlers.end(), consoleHandler
        );
        if (it != _priorityConsoleHandlers.end()) {
            _priorityConsoleHandlers.erase(it);
            return consoleHandler;
        }
        return nullptr;
    }

    void ConsoleManagerService::clear_priority_console_handlers() {
        SKSE::log::warn("clear_priority_console_handlers() called");
        _priorityConsoleHandlers.clear();
    }

    void ConsoleManagerService::run_priority_console_handlers(
        const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_priority_console_handlers() called with commandText: {}, "
            "target: {}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr"
        );
        for (auto& handler : _priorityConsoleHandlers) {
            if (handler->invoke(commandText, target)) return;
        }
    }

    ConsoleHandlerFn ConsoleManagerService::prepend_console_handler(ConsoleHandlerFn consoleHandler
    ) {
        SKSE::log::info("prepend_console_handler() called");
        _consoleHandlers.insert(_consoleHandlers.begin(), consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManagerService::add_console_handler(ConsoleHandlerFn consoleHandler) {
        SKSE::log::info("add_console_handler() called");
        _consoleHandlers.push_back(consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManagerService::remove_console_handler(ConsoleHandlerFn consoleHandler
    ) {
        SKSE::log::warn("remove_console_handler() called");
        auto it = std::find(_consoleHandlers.begin(), _consoleHandlers.end(), consoleHandler);
        if (it != _consoleHandlers.end()) {
            _consoleHandlers.erase(it);
            return consoleHandler;
        }
        return nullptr;
    }

    void ConsoleManagerService::clear_console_handlers() {
        SKSE::log::warn("clear_console_handlers() called");
        _consoleHandlers.clear();
    }

    bool ConsoleManagerService::run_console_handlers(
        const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_console_handlers() called with commandText: {}, target: {}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr"
        );
        for (auto& handler : _consoleHandlers) {
            if (handler->invoke(commandText, target)) return true;
        }
        return false;
    }

    ConsoleListenerFn ConsoleManagerService::add_console_listener(ConsoleListenerFn consoleListener
    ) {
        SKSE::log::info("add_console_listener() called");
        _consoleListeners.push_back(consoleListener);
        return consoleListener;
    }

    ConsoleListenerFn ConsoleManagerService::remove_console_listener(
        ConsoleListenerFn consoleListener
    ) {
        SKSE::log::warn("remove_console_listener() called");
        auto it = std::find(_consoleListeners.begin(), _consoleListeners.end(), consoleListener);
        if (it != _consoleListeners.end()) {
            _consoleListeners.erase(it);
            return consoleListener;
        }
        return nullptr;
    }

    void ConsoleManagerService::clear_console_listeners() {
        SKSE::log::warn("clear_console_listeners() called");
        _consoleListeners.clear();
    }

    void ConsoleManagerService::run_console_listeners(
        const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_console_listeners() called with commandText: {}, target: "
            "{}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr"
        );
        for (auto& listener : _consoleListeners) {
            listener->invoke(commandText, target);
        }
    }

    bool ConsoleManagerService::claim_ownership(ConsoleHandlerFn consoleHandler) {
        SKSE::log::info("claim_ownership() called");
        _ownershipHandlersStack.push_back(consoleHandler);
        return true;
    }

    bool ConsoleManagerService::release_ownership(ConsoleHandlerFn handler) {
        SKSE::log::warn("release_ownership() called");
        if (_ownershipHandlersStack.empty()) return false;

        if (!handler) {
            // Remove the top handler
            _ownershipHandlersStack.pop_back();
            return true;
        } else {
            // Find and remove the specific handler
            auto it =
                std::find(_ownershipHandlersStack.begin(), _ownershipHandlersStack.end(), handler);
            if (it != _ownershipHandlersStack.end()) {
                _ownershipHandlersStack.erase(it);
                return true;
            }
            return false;
        }
    }

    void ConsoleManagerService::clear_ownership_handlers() {
        SKSE::log::warn("clear_ownership_handlers() called");
        _ownershipHandlersStack.clear();
    }

    bool ConsoleManagerService::is_owned() const {
        SKSE::log::trace("is_owned() called");
        return !_ownershipHandlersStack.empty();
    }

    bool ConsoleManagerService::run_owning_handler(
        const char* commandText, RE::TESObjectREFR* target
    ) {
        SKSE::log::debug(
            "run_owning_handler() called with commandText: {}, target: {}",
            commandText ? commandText : "nullptr", target ? "valid" : "nullptr"
        );
        if (_ownershipHandlersStack.empty()) return false;

        // Get the top handler on the stack
        auto handler = _ownershipHandlersStack.back();
        return handler->invoke(commandText, target);
    }
}

// #include <SkyrimScripting/Plugin.h>

// #include "ConsoleManager.h"
// #include "Hook.h"

// auto consoleManager = std::make_unique<SkyrimScripting::Console::ConsoleManager>();

// bool ConsoleCommandHandler(std::string_view commandText, RE::TESObjectREFR* target) {
//     return consoleManager->run(commandText.data(), target);
// }

// SKSEPlugin_OnInputLoaded {
//     Hook::set_command_handler(ConsoleCommandHandler);
//     Hook::install();
// }
