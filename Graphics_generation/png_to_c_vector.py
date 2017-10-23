from PIL import Image
import sys
import numpy as np
import re
import itertools

im = Image.open(sys.argv[1])
scale = float(sys.argv[2])
ver_hor = sys.argv[3]
try:
    mirror = sys.argv[4]
except:
    mirror = "n"
    
if(ver_hor == "v"):
    ver_nhor = 1
elif(ver_hor == "h"):
    ver_nhor = 0
else:
    print "Vertical (v) or horizontal (h) ?"
    quit()
    
if(mirror == "m"):
    mirror = 1
else:
    mirror = 0
       
im = np.array(im)[:,:,0]

if(ver_nhor):
    im = im.transpose()

if(mirror):
    im2 = np.array(im)[::-1,:]
    im2 = im2[1:,:]

    size = im.shape
    sx = size[1]
    sy = 2*size[0] - 1
    im = np.append(im, im2)
    im = im.reshape(2*size[0]-1,size[1])
else:
    size = im.shape
    sx = size[1]
    sy = size[0]

print "Total pixels:", np.sum(im == 0)

im[im == 0] = 1
im[im > 1] = 0




draw_array = []  

def find_first(im):
    for j in itertools.chain(range(sy/2, sy), reversed(range(0, sy/2))):
        for i in reversed(range(0, sx)):
            if im[j,i] == 1:
                return j, i
    return None
    
def add_point(p, type):
    if(ver_nhor):
        draw_array.append((p[1],p[0]))
    else:
        draw_array.append(p)
    im[p] = type
    
def pref_dir(pos):
    y = pos[0] - sy/2
    x = pos[0] - sx/2
    if x <= 0 and y <= 0:
        return np.array([0,1,3,2,6,5,7,8])
    elif x > 0 and y <= 0:
        return np.array([2,5,1,8,0,7,3,6])
    elif x > 0 and y > 0:
        return np.array([8,7,5,6,2,3,1,0])
    elif x <= 0 and y > 0:
        return np.array([6,3,7,0,8,1,5,2])

def pos_chg(pos, dir):
    ca = [\
        (-1,-1), (-1, 0), (-1, 1),\
        ( 0,-1), ( 0, 0), ( 0, 1),\
        ( 1,-1), ( 1, 0), ( 1, 1)]
    cd = ca[dir]
    
    return (pos[0]+cd[0], pos[1]+cd[1])
  
print "Converting image to vector."  
pos = find_first(im)
init_pos = pos
num = 25
while(pos):
    #print pos
    add_point(pos, num)
    neighbors = im[pos[0]-1:pos[0]+2,pos[1]-1:pos[1]+2]
    dirs = pref_dir(pos)
    n = None
    for d in dirs:
        if neighbors.ravel()[d] == 1:
            n = d
            break
    if(n is not None):
        pos=pos_chg(pos, n)
    else:    
        add_point(init_pos, num)
        pos = find_first(im)
        init_pos = pos
        num += 25
        
#print draw_array
t = np.zeros_like(im)
if(ver_nhor):
    t = t.transpose()
    im = im.transpose()
    sx, sy = sy, sx
for p in draw_array:
    t[p] = 255
    
Image.fromarray(t).save("test.bmp")
Image.fromarray(im).save("test2.bmp")

print "Formating vector by scale factor:", scale
draw_array = np.array(draw_array)

draw_array[:,0] -= sy/2
draw_array[:,1] -= sx/2
draw_array = np.array(draw_array*scale, np.int16)
print "Vectorized picture x, then y, array length is:", len(draw_array)

xstr = str(draw_array[:,1])
xstr = xstr.replace("[", "{")
xstr = xstr.replace("]", "};")
print re.sub("(?<=[0123456789])\n", ",\n", re.sub("(?<=[0123456789]) ", ",", xstr))
ystr = str(-draw_array[:,0])
ystr = ystr.replace("[", "{")
ystr = ystr.replace("]", "};")
print re.sub("(?<=[0123456789])\n", ",\n", re.sub("(?<=[0123456789]) ", ",", ystr))
