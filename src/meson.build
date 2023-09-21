pcmlib_sources = [
    'dbus_accessor.cpp',
    'platform_actions.cpp',
    'platform_checks.cpp',
    'platform_config.cpp',
    'log.cpp']

pcmlib = shared_library('pcm',
                        pcmlib_sources,
                        include_directories : inc,
                        dependencies : pcm_dependencies,
                        version : '0.0.0',
                        install : true,
                        install_dir : get_option('libdir'))

pcmd_sources = ['pcm_main.cpp']
pcmd_deps = declare_dependency(link_with : [pcmlib],
  include_directories : inc)

pcmd = executable(
    'pcmd',
    pcmd_sources,
    include_directories: inc,
    dependencies: [
        pcmd_deps,
        sdbusplus_dep,
        sdeventplus_dep,
        phosphor_logging_dep
        # meson.get_compiler('cpp').find_library('pthread'),
        # meson.get_compiler('cpp').find_library('rt')
    ],
    install: true,
)
