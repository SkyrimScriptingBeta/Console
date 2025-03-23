add_rules("mode.debug", "mode.release", "mode.releasedbg")

set_languages("c++23")

option("commonlib")
    set_default("skyrim-commonlib-ae")
option_end()

option("skyrim_version")
    set_default(nil)
option_end()

option("build_plugin")
    set_default(true)
option_end()

option("build_example")
    set_default(true)
option_end()

if not has_config("commonlib") then
    return
end

commonlib_version = get_config("commonlib"):match("skyrim%-commonlib%-(.*)")
skyrim_version    = get_config("skyrim_version") or commonlib_version

if has_config("build_plugin") or has_config("build_example") then
    add_repositories("SkyrimScripting     https://github.com/SkyrimScripting/Packages.git")
    add_repositories("SkyrimScriptingBeta https://github.com/SkyrimScriptingBeta/Packages.git")
    add_repositories("MrowrLib            https://github.com/MrowrLib/Packages.git")
    
    includes("skse_plugin.lua")
    
    add_requires("SkyrimScripting.Plugin", { configs = { commonlib = get_config("commonlib") } })
end

add_requires(get_config("commonlib"))
add_requires("global_macro_functions")
add_requires("function_pointer")
add_requires("collections")
add_requires("unordered_dense")

includes("SkyrimScripting.Console.Shared/xmake.lua")
includes("SkyrimScripting.Console/xmake.lua")

if has_config("build_plugin") then
    includes("SkyrimScripting.Console.SksePlugin/xmake.lua")
end

if has_config("build_example") then
    skse_plugin({
        name = "_Example of using SkyrimScripting.Console",
        deps = {"SkyrimScripting.Console"},
        packages = {"SkyrimScripting.Plugin"}
    })
end
