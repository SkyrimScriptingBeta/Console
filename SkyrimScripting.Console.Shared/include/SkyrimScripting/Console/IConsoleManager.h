#pragma once

#include <RE/Skyrim.h>

#include "FunctionSignatures.h"

namespace SkyrimScripting::Console {

    struct IConsoleManager {
        /**
         * Enable the console.
         */
        virtual bool enable() = 0;

        /**
         * Disable the console.
         */
        virtual void disable() = 0;

        /**
         * Check if the console is enabled.
         */
        virtual bool enabled() const = 0;

        /**
         * Get the selected reference in the console.
         */
        virtual RE::TESObjectREFR* selected_ref() = 0;

        /**
         * Select a reference in the console.
         */
        virtual void select_ref(RE::TESObjectREFR* reference) = 0;

        /**
         * Log a message to the console.
         */
        virtual void log(const char* message) = 0;

        /**
         * Get the last output from the console.
         */
        virtual const char* last_output() = 0;

        /**
         * Run a command natively.
         *
         * Runs the command natively via the Skyrim command without any listeners or handlers.
         *
         * Runs even if the console manager is disabled.
         */
        virtual bool run_native(const char* commandText, RE::TESObjectREFR* target = nullptr) = 0;

        /**
         * Run a command.
         *
         * 1. Runs all console listeners.
         * 2. Runs all priority console handlers. If any returns true, the command is considered
         * 3. Determines the command name from the first part of the command text.
         * 4. Runs all command listeners for the command name.
         * 5. Runs all command handlers for the command name. If any returns true, the command is
         * considered handled.
         * 6. Runs all console handlers. If any returns true, the command is considered handled.
         *
         * Runs even if the console manager is disabled.
         *
         * If ignoreConsoleOwnership is true, any claimed ownership is ignored.
         *
         * If ignoreConsoleOwnership is false and the console is owned, only then owning handler
         * is called.
         */
        virtual bool run(
            const char* commandText, RE::TESObjectREFR* target = nullptr,
            bool ignoreConsoleOwnership = true
        ) = 0;

        /**
         * Run a specific command by command name.
         *
         * Global command listeners and handlers are not run.
         *
         * 1. Runs all command listeners for the command name.
         * 2. Runs all command handlers for the command name. If any returns true, the command is
         * considered handled.
         *
         * Runs even if the console manager is disabled.
         * */
        virtual void run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Command Handlers
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Prepends a command handler to the list of command handlers for the command name.
         */
        virtual ConsoleCommandHandlerFn prepend_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;

        /**
         * Appends a command handler to the list of command handlers for the command name.
         */
        virtual ConsoleCommandHandlerFn add_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;

        /**
         * Removes a command handler from the list of command handlers for the command name.
         *
         * Don't forget to hold onto the ConsoleCommandHandlerFn if you want to remove it later.
         */
        virtual bool remove_command_handler(
            const char* commandName, ConsoleCommandHandlerFn commandHandler
        ) = 0;

        /**
         * Removes all command handlers for the command name.
         */
        virtual void clear_command_handlers(const char* commandName) = 0;

        /**
         * Runs all command handlers for the command name.
         *
         * Runs even if the console manager is disabled.
         */
        virtual void run_command_handlers(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Command Listeners
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Adds a command listener for the command name.
         */
        virtual ConsoleCommandListenerFn add_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) = 0;

        /**
         * Removes a command listener for the command name.
         *
         * Don't forget to hold onto the ConsoleCommandListenerFn if you want to remove it later.
         */
        virtual ConsoleCommandListenerFn remove_command_listener(
            const char* commandName, ConsoleCommandListenerFn commandListener
        ) = 0;

        /**
         * Removes all command listeners for the command name.
         */
        virtual void clear_command_listeners() = 0;

        /**
         * Runs all command listeners for the command name.
         *
         * Does not run any console handlers.
         *
         * Runs even if the console manager is disabled.
         */
        virtual void run_command_listeners(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Priority Console Handlers
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Prepends a priority console handler to the list of priority console handlers.
         *
         * Priority console handlers are run before command handlers.
         */
        virtual ConsoleHandlerFn prepend_priority_console_handler(ConsoleHandlerFn consoleHandler
        ) = 0;

        /**
         * Appends a priority console handler to the list of priority console handlers.
         *
         * Priority console handlers are run before command handlers.
         */
        virtual ConsoleHandlerFn add_priority_console_handler(ConsoleHandlerFn consoleHandler) = 0;

        /**
         * Removes a priority console handler from the list of priority console handlers.
         *
         * Don't forget to hold onto the ConsoleHandlerFn if you want to remove it later.
         */
        virtual ConsoleHandlerFn remove_priority_console_handler(ConsoleHandlerFn consoleHandler
        ) = 0;

        /**
         * Removes all priority console handlers.
         */
        virtual void clear_priority_console_handlers() = 0;

        /**
         * Runs all priority console handlers.
         *
         * Runs even if the console manager is disabled.
         */
        virtual void run_priority_console_handlers(
            const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Console Handlers
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Prepends a console handler to the list of console handlers.
         *
         * These console handlers are run after command handlers.
         *
         * To run a console handler before command handlers, use priority console handlers.
         */
        virtual ConsoleHandlerFn prepend_console_handler(ConsoleHandlerFn consoleHandler) = 0;

        /**
         * Appends a console handler to the list of console handlers.
         *
         * These console handlers are run after command handlers.
         *
         * To run a console handler before command handlers, use priority console handlers.
         */
        virtual ConsoleHandlerFn add_console_handler(ConsoleHandlerFn consoleHandler) = 0;

        /**
         * Removes a console handler from the list of console handlers.
         *
         * Don't forget to hold onto the ConsoleHandlerFn if you want to remove it later.
         */
        virtual ConsoleHandlerFn remove_console_handler(ConsoleHandlerFn consoleHandler) = 0;

        /**
         * Removes all console handlers.
         *
         * This does not remove priority console handlers.
         */
        virtual void clear_console_handlers() = 0;

        /**
         * Runs all console handlers.
         *
         * This does not run priority console handlers.
         *
         * Runs even if the console manager is disabled.
         */
        virtual bool run_console_handlers(
            const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Console Listeners
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Adds a console listener.
         */
        virtual ConsoleListenerFn add_console_listener(ConsoleListenerFn consoleListener) = 0;

        /**
         * Removes a console listener.
         *
         * Don't forget to hold onto the ConsoleListenerFn if you want to remove it later.
         */
        virtual ConsoleListenerFn remove_console_listener(ConsoleListenerFn consoleListener) = 0;

        /**
         * Removes all console listeners.
         */
        virtual void clear_console_listeners() = 0;

        /**
         * Runs all console listeners.
         *
         * Runs even if the console manager is disabled.
         */
        virtual void run_console_listeners(
            const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

        /**
         * /////////////////////////////////////////////////////////////////////////////
         * Manage Ownership
         * /////////////////////////////////////////////////////////////////////////////
         */

        /**
         * Claim ownership of the console.
         *
         * If the console is already owned, the handler is added to the stack of ownership handlers.
         *
         * If the console is not owned, the handler is called with the command text and target.
         * If the handler returns true, the console is considered owned.
         *
         * When the console is owned, the console manager delegates total responsibility
         * for handling console commands to the ownership handler.
         *
         * No console listeners, handlers, or command handlers are run.
         *
         * Note: this does not impact functions such as `run`, `run_command`, `run_native`, etc.
         * It only impacts the underlying handling of the real console commands.
         */
        virtual bool claim_ownership(ConsoleHandlerFn consoleHandler) = 0;

        /**
         * Release ownership of the console.
         *
         * If the console is owned, the ownership handler is removed from the stack of ownership
         * handlers.
         */
        virtual bool release_ownership(ConsoleHandlerFn = nullptr) = 0;

        /**
         * Clear all ownership handlers.
         */
        virtual void clear_ownership_handlers() = 0;

        /**
         * Check if the console is owned.
         */
        virtual bool is_owned() const = 0;

        /**
         * Runs the owning handler, if any.
         *
         * If the console is owned, the owning handler is called with the command text and target.
         *
         * Returns the result of the owning handler.
         */
        virtual bool run_owning_handler(
            const char* commandText, RE::TESObjectREFR* target = nullptr
        ) = 0;

    protected:
        /**
         * Destructor.
         *
         * Protected to prevent deletion through this interface.
         */
        virtual ~IConsoleManager() = default;
    };
}
