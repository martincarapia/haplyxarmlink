project('haplyxarmlink', 'cpp', version : '0.1.0')

# Path to the xArm SDK submodule
xarm_sdk_dir = 'deps/xArm-CPLUS-SDK'

# Include directories for xArm SDK
include_dir = xarm_sdk_dir + '/include'
inc = include_directories(include_dir)

# Compiler flags
add_project_arguments('-std=c++0x', '-Wall', '-fPIC', language : 'cpp')

# Define source files in the same way as in Makefile
xarm_sources = files(
    'deps/xArm-CPLUS-SDK/src/xarm/serial/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/serial/impl/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/serial/impl/list_ports/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/wrapper/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/common/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/debug/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/instruction/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/os/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/port/*.cpp',
    'deps/xArm-CPLUS-SDK/src/xarm/core/*.cpp'
)

# Define output directory for object files
obj_dir = 'builddir/obj/'

# Compile all the source files into object files
xarm_objs = []
foreach src_file : xarm_sources
  xarm_objs += object(src_file, 
    include_directories : inc,
    install : false)  # Do not install these object files
endforeach

# Create the shared library (equivalent to libxarm.so)
lib_xarm = shared_library('xarm', xarm_objs, 
  include_directories : inc,
  install : false)  # Set to true if you want to install the library

# Define example files
example_sources = files('deps/xArm-CPLUS-SDK/example/*.cpp')

# Compile example source files and link them with the library
example_objs = []
foreach example_src : example_sources
  example_objs += object(example_src,
    include_directories : inc,
    install : false)  # Do not install example object files
endforeach

# Create example executables
foreach example_obj : example_objs
  exe_name = example_obj.source_name().splitext()[0]
  executable(exe_name, example_obj,
    link_with : lib_xarm,
    install : true)  # Install example executables
endforeach

# Install the headers
install_headers('deps/xArm-CPLUS-SDK/include/serial', subdir : 'serial')
install_headers('deps/xArm-CPLUS-SDK/include/xarm', subdir : 'xarm')

# Install the library
install_data(lib_xarm, install_dir : 'lib')


