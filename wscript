import os
import sys
import shutil

def remove_files(d):
   try:
      shutil.rmtree(d)
   except Exception as ex:
      print(ex)

def install_symlinks(hdr):

   sym_path =  os.path.abspath('include')

   if not os.geteuid()==0:
      sys.exit('\nERROR: Root Acces is required to execute this script.\n')

   dst_dir = '/usr/local/include/hdf5r'

   remove_files(dst_dir)

   try:
      print('Creating Directory Tree...')
      os.symlink(sym_path,dst_dir)
      print('Header installation complete.')
   except Exception as ex:
      print(ex)

def install_headers(hdr):

   if not os.geteuid()==0:
      sys.exit('\nERROR: Root Acces is required to execute this script.\n')

   src_dir = 'include'
   dst_dir = '/usr/local/include/hdf5r'

   remove_files(dst_dir)

   try:
      print('Creating Directory Tree...')
      shutil.copytree(src_dir,dst_dir,symlinks=True)
      print('Header installation complete.')
   except Exception as ex:
      print(ex)

def options(ctx):
   ctx.load('compiler_c compiler_cxx')

def configure(ctx):
   ctx.load('compiler_c compiler_cxx')
   ctx.check(
      features = 'cxx cxxprogram',
      libpath  = ['/usr/lib/','/usr/local/lib'],
      libs     = ['hdf5','hdf5_hl_cpp','hdf5_cpp'],
      cflags   = ['-march=native','-Wall','-02'],
   )

def build(ctx):

   ## build primary program
   ctx(
         name     = 'write',
         features = 'cxx cxxprogram',
         cxxflags = ['-march=native', '-Wall', '-W'],
         includes = ['.','examples'],
         source   = 'examples/write.cpp',
         target   = 'write',
         lib      = ['hdf5','hdf5_hl_cpp','hdf5_cpp'],
   )

   ctx(
         name     = 'read',
         features = 'cxx cxxprogram',
         cxxflags = ['-march=native', '-Wall', '-W'],
         includes = ['.','examples'],
         source   = 'examples/read.cpp',
         target   = 'read',
         lib      = ['hdf5','hdf5_hl_cpp','hdf5_cpp'],
   )
