libbdm_files = [
	'bdm/exception/error_info.cxx',
	'bdm/exception/bdm_exception.cxx',
	'bdm/logger/log_console_sink.cxx',
	'bdm/device/mcp3004.cxx',
	'bdm/device/spi/spidev.cxx',
]

vcs_rev = vcs_tag(input: 'version.hxx.in', output: 'version.hxx',
	fallback: meson.project_version())

libbdm = library('bdm', [libbdm_files, vcs_rev],
	dependencies: [boost_dep, g3logger_dep],
	include_directories: include_directories(['.']),
	cpp_args: project_warnings,
	cpp_pch: 'pch/stdafx.h')

libbdm_dep = declare_dependency(
	sources: vcs_rev,
	link_with: libbdm,
	dependencies: [boost_dep, g3logger_dep],
	include_directories: include_directories(['.']))

main_src = [
	'bdmbin/logger.cxx',
]
executable('test_spi', ['test_spi.cxx', main_src],
	dependencies: [libbdm_dep],
	cpp_args: project_warnings)
