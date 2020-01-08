# https://docs.godotengine.org/zh_CN/latest/development/compiling/compiling_with_mono.html

platform=osx
scons p=$platform tools=yes module_mono_enabled=yes mono_glue=no

./bin/godot.$platform.tools.64.mono --generate-mono-glue modules/mono/glue

# Editor
scons p=$platform target=release_debug tools=yes module_mono_enabled=yes
# Export templates
scons p=$platform target=release_debug tools=no module_mono_enabled=yes
scons p=$platform target=release tools=no module_mono_enabled=yes