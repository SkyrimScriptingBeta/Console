skse_plugin({
    target = "SkyrimScripting.Console SKSE Plugin",
    name = "SkyrimScripting.Console",
    version = "0.0.1",
    author = "Mrowr Purr",
    email = "mrowr.purr@gmail.com",
    deps = {"SkyrimScripting.Console"},
    packages = {"SkyrimScripting.Plugin", "SkyrimScripting.Services", "collections", "unordered_dense"},
    src = "src/*.cpp",
    include = "include"
})
if skyrim_version == "ae" then
    add_defines("SKYRIM_VERSION_AE")
elseif skyrim_version == "se" then
    add_defines("SKYRIM_VERSION_SE")
elseif skyrim_version == "vr" then
    add_defines("SKYRIM_VERSION_VR")
end
