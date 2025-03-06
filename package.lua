-- This is an example of a package() which can be shared in any xmake package registry

package("my_static_library")
    set_homepage("https://github.com/SkyrimScriptingBeta/ExampleStaticLibrary")
    set_description("My library which uses CommonLib")
    set_license("0BSD")

    add_urls("https://github.com/SkyrimScriptingBeta/ExampleStaticLibrary/archive/refs/tags/$(version).tar.gz",
            "https://github.com/SkyrimScriptingBeta/ExampleStaticLibrary.git")

    -- Require users to specify which CommonLib package they want
    add_configs("commonlib", {description = "Specify package name for commonlib dependency", default = "", type = "string"})

    on_load(function (package)
        local commonlib = package:config("commonlib")
        if not commonlib then
            raise("You must specify a CommonLib version, e.g., `xmake f --commonlib=skyrim-commonlib-ae`")
        end
    end)

    on_install(function (package)
        import("package.tools.xmake").install(package, {
            commonlib = package:config("commonlib"),
            -- Hardcoded options (disabling options which are 'true' for local development)
            require_commonlib = false,
            include_repo_skyrimscripting = false,
            build_example = false,
            build_papyrus_scripts = false
        })
    end)
