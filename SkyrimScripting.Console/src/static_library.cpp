#include <SkyrimScripting/Services.h>

#include "SkyrimScripting/Console/HelperFunctions.h"

namespace SkyrimScripting::Console::HelperFunctions {
    IConsoleManagerService* GetConsoleManager() {
        return GetService<IConsoleManagerService>(IConsoleManagerService::SERVICE_NAME);
    }
}
