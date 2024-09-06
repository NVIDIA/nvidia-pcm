project('c++ nvidia-pcm', 'cpp',
  version : '0.1',
  license : 'MIT',
  meson_version: '>=0.58.0',
  default_options: [
            'cpp_std=c++20'])

# Project related links

project_pretty_name = 'nvidia-platform-configuration-manager'
project_url = 'https://gitlab-master.nvidia.com/dgx/nvidia-pcm'
project_issues_url = project_url + '/issues/new'
lang = 'cpp'

#summary('Issues',project_issues_url, section: 'Report Issues')

cxx = meson.get_compiler(lang)

## Set global arguments
add_global_arguments('-DSOME_TOKEN=value', language : lang)

# Validate the c++ Standard
if get_option('cpp_std') != 'c++20'
    error('This project requires c++20 support '+ get_option('cpp_std'))
endif

debug_log_level = get_option('debug_log')
if debug_log_level != 0
    add_project_arguments('-DDEF_DBG_LEVEL=' + debug_log_level.to_string(), language : lang)
endif

systemd_dep = dependency('systemd')
sdbusplus_dep = dependency('sdbusplus', required: false)
sdbusplus_proj = dependency('', required: false)
# phosphor_logging_dep = cxx.find_library('phosphor_logging')

phosphor_logging_dep = dependency(
    'phosphor-logging', required: true)

if not sdbusplus_dep.found() or sdbusplus_dep.type_name() == 'internal'
    sdbusplus_proj = subproject('sdbusplus')
endif
if not sdbusplus_dep.found()
    sdbusplus_dep = sdbusplus_proj.get_variable('sdbusplus_dep')
endif

if sdbusplus_proj.found()
    sdbusplusplus_prog = sdbusplus_proj.get_variable('sdbusplusplus_prog')
    sdbuspp_gen_meson_prog = sdbusplus_proj.get_variable(
        'sdbuspp_gen_meson_prog')
else
    sdbusplusplus_prog = find_program('sdbus++', native: true)
    sdbuspp_gen_meson_prog = find_program('sdbus++-gen-meson', native: true)
endif

sdeventplus_dep = dependency(
    'sdeventplus',
    required: true,
)

# Add all the dependencies
pcm_dependencies = []

pcm_dependencies += sdbusplus_dep
pcm_dependencies += phosphor_logging_dep
# #pcm_dependencies += dependency('glib-2.0')
# pcm_dependencies += dependency('threads')
# pcm_dependencies += meson.get_compiler('cpp').find_library('pthread')
# pcm_dependencies += meson.get_compiler('cpp').find_library('rt')

if cxx.has_header('nlohmann/json.hpp')
    pcm_dependencies += declare_dependency()
endif

# Add Include Directory
inc = include_directories('include')

# subdir for meson project
subdir('include')
subdir('src')

# Pkg-config
pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : pcmlib,
                 version : '0.1',
                 name : 'pcm',
                 filebase : 'pcm',
                 description : 'nvidia-pcm')

pcmlib_sources = [
    'src/dbus_accessor.cpp',
    'src/platform_actions.cpp',
    'src/platform_checks.cpp',
    'src/platform_config.cpp',
    'src/log.cpp']

