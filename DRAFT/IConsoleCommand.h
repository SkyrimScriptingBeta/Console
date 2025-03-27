#pragma once

#include <RE/Skyrim.h>

namespace SkyrimScripting::Console {

    struct IConsoleCommand {
        virtual const char*        text() const = 0;
        virtual RE::TESObjectREFR* ref() const  = 0;

    protected:
        virtual ~IConsoleCommand() = default;
    };
}
