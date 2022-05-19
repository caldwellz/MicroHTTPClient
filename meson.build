project('', ['c', 'cpp']
  , version : '0.0.1'
  , default_options : ['buildtype=debug', 'warning_level=3', 'cpp_eh=none', 'cpp_rtti=false', 'cpp_std=c++14', 'prefix=/usr/local'])


# Add DEBUG/NDEBUG defines
if get_option('buildtype').to_lower() == 'debug'
  add_project_arguments('-DDEBUG', language: ['c', 'cpp'])
else
  add_project_arguments('-DNDEBUG', language: ['c', 'cpp'])
endif


# Run subdirectory scripts
subdir('include')
subdir('src')
if get_option('BUILD_TESTS')
  subdir('tests')
endif
