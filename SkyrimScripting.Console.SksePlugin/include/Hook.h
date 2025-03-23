#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace Hook {
    void set_command_handler(
        std::function<bool(std::string_view, RE::TESObjectREFR*)> a_commandHandler
    );
    void install();
}
