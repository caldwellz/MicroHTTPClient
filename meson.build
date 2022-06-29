project('MHC', ['c']
  , version : '0.1.0'
  , default_options : ['buildtype=debug', 'warning_level=3', 'c_std=c99', 'prefix=/usr/local'])


# Add DEBUG/NDEBUG defines
if get_option('buildtype').to_lower() == 'debug'
  add_project_arguments('-DDEBUG', language: ['c'])
else
  add_project_arguments('-DNDEBUG', language: ['c'])
endif


# Run subdirectory scripts
subdir('include')
subdir('src')
if get_option('BUILD_TESTS')
  subdir('tests')
endif
