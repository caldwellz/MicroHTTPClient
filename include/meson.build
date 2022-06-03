# Add this directory to the include path
lib_incdir = include_directories('.')


# Fill in config.h with the build options
conf_data = configuration_data()
conf_data.set('BUILD_SHARED', get_option('default_library').contains('shared'))
conffile = configure_file(configuration : conf_data,
    input : 'config.h.in',
    output : 'config.h')


# Install headers - add others to lib_headers below
lib_headers = [conffile, 'version.h', 'MHC_config.h', 'MHC_context.h', 'MHC_helpers.h', 'MHC_params.h', 'MHC_requests.h', 'MHC_response.h', 'MHC_stringutils.h']
install_headers(lib_headers, subdir : meson.project_name())
