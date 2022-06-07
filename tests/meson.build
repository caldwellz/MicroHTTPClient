tests = [
    ['version', '.c'],
    ['get-request', '.c'],
    ['request-format', '.c']
]

# Find and use required libraries
ccomp = meson.get_compiler('c')
libsdlnet_dep = dependency('libSDL2_net', required : false)
if not libsdlnet_dep.found()
    libsdlnet_dep = ccomp.find_library('libSDL2_net', has_headers : ['SDL2/SDL_net.h'], required : true)
endif
test_deps = lib_deps + [libsdlnet_dep]

foreach t : tests
    exe = executable(t[0], t[0] + t[1], dependencies : test_deps, link_with : lib_target, include_directories : lib_incdir)
    test(t[0], exe)
endforeach
