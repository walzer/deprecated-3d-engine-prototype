
import os,sys
import config



def png_convert_pvr(src_file):
    cmd = ""
    if os.name =="nt":
        print "can't find png_convert_pvr tool in windows"
    else:
        texturetool = "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/texturetool "
        out_file = "%s.pvr"%src_file[:-4]
        cmd = "%s -e PVRTC -o %s -f PVR %s"%(texturetool, out_file, src_file)
        print "cmd=%s"%cmd
        iret = os.system(cmd)
        if iret >= 0:
            print "%s ok"%src_file
        else:
            print "%s false"%src_file
    
 

def convert_call_back(arg,dirname,names):
    for name in names:
        file = os.path.join(dirname, name)
        if not file.endswith(".png"):
            continue
        png_convert_pvr(file)
        break;
    
def convert(dir_list):
    print dir_list
    for dir in dir_list:
        print dir
        os.path.walk(dir,convert_call_back,())

if __name__ == "__main__":

     print config.g_resources_dir
	 
     convert([config.g_resources_dir])
 