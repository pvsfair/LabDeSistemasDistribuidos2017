from distutils.core import setup, Extension

src = 'LabDeSistemasDistribuidos2017/main.cpp'

module1 = Extension('spam',
                    sources = [src],
		    extra_compile_args=['-std=c++11','-fopenmp'],
		    extra_link_args = ['-lgomp'])

setup (name = 'spam',
       version = '1.0',
       description = 'This is a demo package',
       ext_modules = [module1])
