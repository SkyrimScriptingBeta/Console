#pragma once

#include <RE/Skyrim.h>
#include <function_pointer.h>

namespace SkyrimScripting::Console {

    using ConsoleHandlerFn =
        IFunctionPointer<bool(const char* commandText, RE::TESObjectREFR* target)>*;

    using ConsoleListenerFn =
        IFunctionPointer<void(const char* commandText, RE::TESObjectREFR* target)>*;

    using ConsoleCommandHandlerFn = IFunctionPointer<
        bool(const char* command, const char* commandText, RE::TESObjectREFR* target)>*;

    using ConsoleCommandListenerFn = IFunctionPointer<
        void(const char* command, const char* commandText, RE::TESObjectREFR* target)>*;

}