add_rules("mode.debug", "mode.release", "mode.releasedbg")

set_languages("c++23")

option("commonlib")
    set_description("Which CommonLib package to use")
    set_default("skyrim-commonlib-ae")
option_end()

option("require_commonlib")
    set_description("If true, add_requires the CommonLib package")
    set_default(true)
option_end()

option("include_repo_skyrimscripting")
    set_description("If true, add the SkyrimScripting repository during build")
    set_default(true)
option_end()

option("build_example")
    set_description("Build example project using this library")
    set_default(true)
option_end()

option("build_papyrus_scripts")
    set_description("Build Papyrus scripts")
    set_default(false)
option_end()

library_name = "MyStaticLibrary"

-- Example SKSE plugin using the static library
mod_info = {
    name = "Test plugin for " .. library_name,
    version = "0.0.1",
    author = "Mrowr Purr",
    email = "mrowr.purr@gmail.com",
    mod_files = {"Scripts"}
}

skyrim_versions = {"ae", "se", "ng", "vr"}

if has_config("include_repo_skyrimscripting") then
    add_repositories("SkyrimScripting https://github.com/SkyrimScripting/Packages.git")
end

if has_config("require_commonlib") then
    add_requires(get_config("commonlib"))
end

if has_config("commonlib") then
    print("Building using CommonLib package: " .. get_config("commonlib"))
    target(library_name)
        set_kind("static")
        add_files("src/*.cpp")
        add_includedirs("include", { public = true }) -- Your library's own include path
        add_headerfiles("include/(**.h)")
        if has_config("commonlib") then
            add_packages(get_config("commonlib"), { public = true })
        end
end

if has_config("build_example") then
    print("Building example project")
    includes("skse.lua")
end

if has_config("build_papyrus_scripts") then
    includes("papyrus.lua")
end
