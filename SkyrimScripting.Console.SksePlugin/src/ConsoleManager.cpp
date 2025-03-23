#include <SKSE/SKSE.h>
#include <SkyrimScripting/Console/IConsoleCommand.h>
#include <SkyrimScripting/Console/IConsoleManager.h>
#include <collections.h>

#include <atomic>
#include <string>

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

        bool run_command(
            const char* commandName, const char* commandText, RE::TESObjectREFR* target = nullptr
        ) override {
            auto it = _commandHandlers.find(commandName);
            if (it == _commandHandlers.end()) return false;
            return it->second->invoke(commandName, commandText, target);
        }

        void actual_run_used_by_hook(const char* commandText, RE::TESObjectREFR* target = nullptr) {
            //
        }
    };
}
