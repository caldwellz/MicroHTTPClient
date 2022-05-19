tests = [
    ['version', '.c']]

# Test dependencies
test_deps = lib_deps + []

foreach t : tests
    exe = executable(t[0], t[0] + t[1], dependencies : test_deps, link_with : lib_target, include_directories : lib_incdir)
    test(t[0], exe)
endforeach
