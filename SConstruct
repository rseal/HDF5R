import os
env = Environment()

#define directories
include_dir = '/usr/local/include/HDF5'

#install development headers with 'install' argument
headers = [Glob('include/*')]
install = env.Install(dir = include_dir, source = headers)
env.Alias('install-headers', include_dir, install)

#call all sub-build scripts
build = env.SConscript('examples/SConscript')

