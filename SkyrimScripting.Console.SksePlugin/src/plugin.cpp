#include <SKSE/SKSE.h>
#include <SkyrimScripting/Console/IConsoleCommand.h>
#include <SkyrimScripting/Console/IConsoleManager.h>
#include <collections.h>

#include <atomic>
#include <string>

namespace SkyrimScripting::Console {

    namespace Hook {

        std::atomic<bool> _hookInstalled = false;

        void CompileAndRun(
            RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name,
            RE::TESObjectREFR* a_targetRef
        );

        inline REL::Relocation<decltype(CompileAndRun)> _CompileAndRun;

        void InstallHook() {
            if (_hookInstalled.exchange(true)) return;

#ifdef SKYRIM_VERSION_AE
            REL::Relocation<std::uintptr_t> hookPoint{REL::ID(52952), 0x52};
#elif SKYRIM_VERSION_SE
            REL::Relocation<std::uintptr_t> hookPoint{REL::ID(52065), 0xE2};
#elif SKYRIM_VERSION_VR
            REL::Offset hookPoint{static_cast<std::uint64_t>(0x90E2D2)};
#else
    #error Unspecified Skyrim version
#endif

            auto& trampoline = SKSE::GetTrampoline();
            trampoline.create(14);
            _CompileAndRun = trampoline.write_call<5>(hookPoint.address(), CompileAndRun);
        }

        void CompileAndRun(
            RE::Script* script, RE::ScriptCompiler* compiler, RE::COMPILER_NAME compilerName,
            RE::TESObjectREFR* targetRef
        ) {
            SKSE::log::info("HELLO FROM OVERRIDEN CONSOLE COMMAND!");
            auto commandText = script->GetCommand();
            SKSE::log::info("The Command: {}", commandText);

            // for (auto& callback : CommandRegistration::CommandCallbacks)
            //     if (callback(commandText)) return;
            _CompileAndRun(script, compiler, compilerName, targetRef);
        }
    }

    class ConsoleCommand : IConsoleCommand {
        std::string        _text;
        RE::TESObjectREFR* _ref;

    public:
        ConsoleCommand(const char* text, RE::TESObjectREFR* ref) : _text(text), _ref(ref) {}

        const char*        text() const override { return _text.c_str(); }
        RE::TESObjectREFR* ref() const override { return _ref; }
    };

    class ConsoleManager : IConsoleManager {
        std::atomic<bool>                                      _running = false;
        std::vector<ConsoleHandlerFn>                          _ownershipHandlersStack;
        std::vector<ConsoleListenerFn>                         _consoleListeners;
        std::vector<ConsoleHandlerFn>                          _consoleHandlers;
        collections_map<std::string, ConsoleCommandHandlerFn>  _commandHandlers;
        collections_map<std::string, ConsoleCommandListenerFn> _commandListeners;

    public:
        bool start() override {
            if (_running) return false;
            InstallHook();
            _running = true;
            return true;
        }

        void stop() override {
            if (!_running) return;
            _running = false;
        }

        bool running() const override { return _running; }

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
