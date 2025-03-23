#include "Hook.h"

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <atomic>
#include <functional>

namespace Hook {

    std::atomic<bool>                                         _hookInstalled = false;
    std::function<bool(std::string_view, RE::TESObjectREFR*)> _commandHandler;

    void set_command_handler(
        std::function<bool(std::string_view, RE::TESObjectREFR*)> commandHandler
    ) {
        _commandHandler = commandHandler;
    }

    void CompileAndRun(
        RE::Script* a_script, RE::ScriptCompiler* a_compiler, RE::COMPILER_NAME a_name,
        RE::TESObjectREFR* a_targetRef
    );

    inline REL::Relocation<decltype(CompileAndRun)> _CompileAndRun;

    void install() {
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
        auto commandText = script->GetCommand();
        if (_commandHandler)
            if (_commandHandler(commandText, targetRef)) return;
        _CompileAndRun(script, compiler, compilerName, targetRef);
    }
}
