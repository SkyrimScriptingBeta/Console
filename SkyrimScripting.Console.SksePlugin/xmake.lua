skse_plugin({
    name = "SkyrimScripting.Console.SksePlugin",
    version = "0.0.1",
    author = "Mrowr Purr",
    email = "mrowr.purr@gmail.com",
    deps = {"SkyrimScripting.Console.Shared"},
    packages = {"SkyrimScripting.Plugin", "collections", "unordered_dense"}
})
if skyrim_version == "ae" then
    add_defines("SKYRIM_VERSION_AE")
elseif skyrim_version == "se" then
    add_defines("SKYRIM_VERSION_SE")
elseif skyrim_version == "vr" then
    add_defines("SKYRIM_VERSION_VR")
end
