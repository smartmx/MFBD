from building import *

cwd = GetCurrentDir()

# init src
src = []
src = Glob('*.c')

if GetDepend(['PKG_USING_MFBD_DEMO_RTT']):
    src += Glob("examples/mfbd_demo_rtt.c")

CPPPATH = [cwd]

group = DefineGroup('mfbd', src, depend = ['PKG_USING_MFBD_DEMO_RTT'], CPPPATH = CPPPATH)

Return('group')
