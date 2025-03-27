#pragma once

#include <SkyrimScripting/Services/Initialize.h>

namespace SkyrimScripting::Console {
    /**
     * The `SkyrimScripting::Console::Initialize` function **MUST** be called during the plugin's
     * initialization.
     *
     * > Must call this BEFORE the `kPostLoad` event.
     */
    inline void Initialize() { SkyrimScripting::Services::Initialize(); }
}
