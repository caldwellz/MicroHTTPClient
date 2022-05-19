# Add this directory to the include path
lib_incdir = include_directories('.')


# Fill in config.h with the build options
conf_data = configuration_data()
conf_data.set('BUILD_SHARED', get_option('default_library').contains('shared'))
conf_data.set('OPT_USE_EXCEPTIONS', not get_option('cpp_eh').contains('none'))
conf_data.set('OPT_USE_RTTI', get_option('cpp_rtti'))
conffile = configure_file(configuration : conf_data,
    input : 'config.h.in',
    output : 'config.h')


# Install headers - add others to lib_headers below
lib_headers = [conffile, 'version.h']
install_headers(lib_headers, subdir : meson.project_name())
