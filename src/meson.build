libbdm_files = [
	'bdm/device/demultiplexer.cxx',
	'bdm/device/gpio/gpio_wrapper.cxx',
	'bdm/device/mcp3004.cxx',
	'bdm/device/spi/spidev.cxx',
	'bdm/exception/error_info.cxx',
	'bdm/exception/bdm_exception.cxx',
	'bdm/logger/log_console_sink.cxx',
	'bdm/task/sound_buffer.cxx',
	'bdm/task/delay_estimation.cxx',
	'bdm/task/position_estimation.cxx',
]

inc = include_directories(['.'])

libbdm = library('bdm', [libbdm_files],
	dependencies: [boost_dep, g3logger_dep, wiringpi_dep],
	include_directories: inc,
	cpp_args: project_warnings,
	cpp_pch: 'pch/stdafx.h')

libbdm_dep = declare_dependency(
	link_with: libbdm,
	dependencies: [boost_dep, g3logger_dep],
	include_directories: inc)

libbdmbin_files = [
	'bdmbin/logger.cxx',
]
libbdmbin = library('bdmbin', libbdmbin_files,
	dependencies: [libbdm_dep],
	cpp_args: project_warnings,
	include_directories: inc)
libbdmbin_dep = declare_dependency(
	link_with: libbdmbin,
	dependencies: [libbdm_dep],
	include_directories: inc)

executable('test_spi', 'test_spi.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
executable('test_gpio', 'test_gpio.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
executable('test_demux', 'test_demux.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
executable('test_microphone', 'test_microphone.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
executable('test_delay', 'test_delay.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
executable('main', 'main.cxx',
	dependencies: [libbdm_dep, libbdmbin_dep],
	cpp_args: project_warnings)
