# Template for static library _(which requires CommonLib as a dependency)_

This is a template for a static library project that depends on CommonLibSSE or CommonLibVR.

At this time, the template is `xmake`-based.

> At some point in the future, support for `CMake` may be added.

## Building for different Skyrim versions

In the root `xmake.lua`, setup one or multiple skyrim versions to build for:

```lua
skyrim_versions = {"ae", "se", "ng", "vr"}
```

> This whole template assumes that you'll be using the CommonLibSSE / CommonLibVR xmake packages from the [Skyrim Scripting Package registry](https://github.com/SkyrimScripting/Packages).

## `xmake` build targets

| Target                           | Description                                                                    |
| -------------------------------- | ------------------------------------------------------------------------------ |
| Build Papyrus Scripts            | Builds Papyrus scripts (requires `pyro` in `PATH`)<br>**Disabled by default.** |
| `[Library Name]`                 | Builds the static library                                                      |
| `StaticLibrary-[Skyrim Version]` | Automatically configured dependency for SKSE example plugin.                   |
| `_SksePlugin-[Skyrim Version]`   | Builds an example SKSE plugin.<br>**Enabled by default.**                      |

> Note: the `_` prefix for `_SksePlugin` is a hack for `compile_commands` for `clangd` to use the SKSE plugin in VS Code as the main target used for autocompletion.
>
> It's recommended to have an example SKSE plugin for two reasons:
> 1. To test our your static library in the game
> 2. To test that your library works from an SKSE plugin
> 3. It can be beneficial for autocomplation in VS Code

## Configuring example SKSE plugin



## Distributing the library

See [`package.lua`](package.lua) for an example `xmake` package definition.

You can also look at example packages here:

- https://github.com/SkyrimScriptingBeta/Packages

---

> Uses [`xmake`][] as build system.
> Also requires [`pyro`][] (should be installed in `PATH`).
> The `Scripts/Bindings` folder is for using [`BIND`][].


[`xmake`]: https://xmake.io
[`pyro`]: https://wiki.fireundubh.com/pyro
[`BIND`]: https://github.com/SkyrimScripting/Bind
