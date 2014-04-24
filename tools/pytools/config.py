#!/usr/bin/python

"""
config 
"""
import os,sys




#common config
def script_path():
    path = os.path.realpath(sys.path[0])
    if os.path.isfile(path):
        path = os.path.dirname(path)
    return os.path.abspath(path)

g_current_dir = script_path()
g_project_dir = os.path.normpath(os.path.join(g_current_dir, "../../"))


g_resources_dir = os.path.join(g_project_dir, "EngineDemo/Resources/3d")




