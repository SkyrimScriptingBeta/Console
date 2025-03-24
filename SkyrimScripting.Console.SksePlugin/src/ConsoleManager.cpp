#include "ConsoleManager.h"

#include <string>

namespace SkyrimScripting::Console {

    bool ConsoleManager::enable() {
        _enabled = true;
        return true;
    }

    void ConsoleManager::disable() { _enabled = false; }

    bool ConsoleManager::enabled() const { return _enabled; }

    /**
     * `selected_ref`, `select_ref`, and `run_native` functions source come from ConsoleUtilSSE
     * - https://github.com/VersuchDrei/ConsoleUtilSSE
     * - License: MIT
     *
     * _Provided here on ConsoleManager for convenience_
     */
    RE::TESObjectREFR* ConsoleManager::selected_ref() {
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
    void ConsoleManager::select_ref(RE::TESObjectREFR* reference) {
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

    void ConsoleManager::log(const char* message) {
        if (!message) return;
        auto console = RE::ConsoleLog::GetSingleton();
        if (console) console->Print(message);
    }

    const char* ConsoleManager::last_output() {
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
    bool ConsoleManager::run_native(const char* commandText, RE::TESObjectREFR* target) {
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

    bool ConsoleManager::run(
        const char* commandText, RE::TESObjectREFR* target, bool ignoreConsoleOwnership
    ) {
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
        run_console_handlers(commandText, target);

        return false;
    }

    void ConsoleManager::run_command(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        if (!target) target = selected_ref();

        // Run command listeners
        run_command_listeners(commandName, commandText, target);

        // Run command handlers
        run_command_handlers(commandName, commandText, target);
    }

    ConsoleCommandHandlerFn ConsoleManager::prepend_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        _commandHandlers.insert_or_assign(std::string(commandName), commandHandler);
        return commandHandler;
    }

    ConsoleCommandHandlerFn ConsoleManager::add_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        _commandHandlers.insert_or_assign(std::string(commandName), commandHandler);
        return commandHandler;
    }

    bool ConsoleManager::remove_command_handler(
        const char* commandName, ConsoleCommandHandlerFn commandHandler
    ) {
        auto it = _commandHandlers.find(commandName);
        if (it != _commandHandlers.end() && it->second == commandHandler) {
            _commandHandlers.erase(it);
            return true;
        }
        return false;
    }

    void ConsoleManager::clear_command_handlers(const char* commandName) {
        _commandHandlers.erase(commandName);
    }

    void ConsoleManager::run_command_handlers(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        auto it = _commandHandlers.find(commandName);
        if (it != _commandHandlers.end()) {
            it->second->invoke(commandName, commandText, target);
        }
    }

    ConsoleCommandListenerFn ConsoleManager::add_command_listener(
        const char* commandName, ConsoleCommandListenerFn commandListener
    ) {
        _commandListeners.insert_or_assign(std::string(commandName), commandListener);
        return commandListener;
    }

    ConsoleCommandListenerFn ConsoleManager::remove_command_listener(
        const char* commandName, ConsoleCommandListenerFn commandListener
    ) {
        auto it = _commandListeners.find(commandName);
        if (it != _commandListeners.end() && it->second == commandListener) {
            _commandListeners.erase(it);
            return commandListener;
        }
        return nullptr;
    }

    void ConsoleManager::clear_command_listeners() { _commandListeners.clear(); }

    void ConsoleManager::run_command_listeners(
        const char* commandName, const char* commandText, RE::TESObjectREFR* target
    ) {
        auto it = _commandListeners.find(commandName);
        if (it != _commandListeners.end()) {
            it->second->invoke(commandName, commandText, target);
        }
    }

    ConsoleHandlerFn ConsoleManager::prepend_priority_console_handler(
        ConsoleHandlerFn consoleHandler
    ) {
        _priorityConsoleHandlers.insert(_priorityConsoleHandlers.begin(), consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManager::add_priority_console_handler(ConsoleHandlerFn consoleHandler) {
        _priorityConsoleHandlers.push_back(consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManager::remove_priority_console_handler(ConsoleHandlerFn consoleHandler
    ) {
        auto it = std::find(
            _priorityConsoleHandlers.begin(), _priorityConsoleHandlers.end(), consoleHandler
        );
        if (it != _priorityConsoleHandlers.end()) {
            _priorityConsoleHandlers.erase(it);
            return consoleHandler;
        }
        return nullptr;
    }

    void ConsoleManager::clear_priority_console_handlers() { _priorityConsoleHandlers.clear(); }

    void ConsoleManager::run_priority_console_handlers(
        const char* commandText, RE::TESObjectREFR* target
    ) {
        for (auto& handler : _priorityConsoleHandlers) {
            if (handler->invoke(commandText, target)) return;
        }
    }

    ConsoleHandlerFn ConsoleManager::prepend_console_handler(ConsoleHandlerFn consoleHandler) {
        _consoleHandlers.insert(_consoleHandlers.begin(), consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManager::add_console_handler(ConsoleHandlerFn consoleHandler) {
        _consoleHandlers.push_back(consoleHandler);
        return consoleHandler;
    }

    ConsoleHandlerFn ConsoleManager::remove_console_handler(ConsoleHandlerFn consoleHandler) {
        auto it = std::find(_consoleHandlers.begin(), _consoleHandlers.end(), consoleHandler);
        if (it != _consoleHandlers.end()) {
            _consoleHandlers.erase(it);
            return consoleHandler;
        }
        return nullptr;
    }

    void ConsoleManager::clear_console_handlers() { _consoleHandlers.clear(); }

    void ConsoleManager::run_console_handlers(const char* commandText, RE::TESObjectREFR* target) {
        for (auto& handler : _consoleHandlers) {
            if (handler->invoke(commandText, target)) return;
        }
    }

    ConsoleListenerFn ConsoleManager::add_console_listener(ConsoleListenerFn consoleListener) {
        _consoleListeners.push_back(consoleListener);
        return consoleListener;
    }

    ConsoleListenerFn ConsoleManager::remove_console_listener(ConsoleListenerFn consoleListener) {
        auto it = std::find(_consoleListeners.begin(), _consoleListeners.end(), consoleListener);
        if (it != _consoleListeners.end()) {
            _consoleListeners.erase(it);
            return consoleListener;
        }
        return nullptr;
    }

    void ConsoleManager::clear_console_listeners() { _consoleListeners.clear(); }

    void ConsoleManager::run_console_listeners(const char* commandText, RE::TESObjectREFR* target) {
        for (auto& listener : _consoleListeners) {
            listener->invoke(commandText, target);
        }
    }

    bool ConsoleManager::claim_ownership(ConsoleHandlerFn consoleHandler) {
        _ownershipHandlersStack.push_back(consoleHandler);
        return true;
    }

    bool ConsoleManager::release_ownership(ConsoleHandlerFn handler) {
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

    void ConsoleManager::clear_ownership_handlers() { _ownershipHandlersStack.clear(); }

    bool ConsoleManager::is_owned() const { return !_ownershipHandlersStack.empty(); }

    bool ConsoleManager::run_owning_handler(const char* commandText, RE::TESObjectREFR* target) {
        if (_ownershipHandlersStack.empty()) return false;

        // Get the top handler on the stack
        auto handler = _ownershipHandlersStack.back();
        return handler->invoke(commandText, target);
    }
}
