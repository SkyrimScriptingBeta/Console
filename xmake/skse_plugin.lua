function skse_plugin(mod_info)
    local commonlib_version = get_config("commonlib"):match("skyrim%-commonlib%-(.*)")
    local mods_folders = os.getenv("SKYRIM_MODS_FOLDERS")

    if mods_folders then
        mod_info.mods_folders = mods_folders
    else
        print("SKYRIM_MODS_FOLDERS environment variable not set")
    end
    
    target(mod_info.name)
        set_basename(mod_info.name .. "-" .. commonlib_version:upper())
        add_packages(get_config("commonlib"))
        add_rules("@skyrim-commonlib-" .. commonlib_version .. "/plugin", {
            mod_name     = mod_info.name .. " (" .. commonlib_version:upper() .. ")",
            mods_folders = mod_info.mods_folders or "",
            mod_files    = mod_info.mod_files,
            name         = mod_info.name,
            version      = mod_info.version,
            author       = mod_info.author,
            email        = mod_info.email
        })
        add_files(mod_info.src or "src/*.cpp")
        if mod_info.include then
            add_includedirs(mod_info.include, { public = true })
        end
        for _, dep in ipairs(mod_info.deps or {}) do
            add_deps(dep, { public = true })
        end
        for _, package in ipairs(mod_info.packages or {}) do
            add_packages(package, { public = true })
        end
end
