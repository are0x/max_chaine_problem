import Image
import os,sys

class PuyoFieldMaker:
    def __init__(self):    
        self.fielddata = []
        #image load
        self.fieldimg = Image.open("field3.jpg")
        self.puyoimg = Image.open("puyo.gif")
        w=self.puyoimg.size[0]/6
        h=self.puyoimg.size[1]/18
        self.colorpuyo=[]
        curw=0
        curh=0
        idx =0
        while idx<=5:
            tmp=[]
            curh=0
            while curh<self.puyoimg.size[1]:
                im = self.puyoimg.crop((curw,curh,curw+w,curh+h))
                im=im.convert("RGBA")
                tmp.append(im)
                curh+=h
            self.colorpuyo.append(tmp)
            idx+=1
            curw+=w
        #trans init
        self.colortoint={"R":0,"B":1,"Y":2,"G":3,"P":4,"O":5}
        #up right down left
        #1,2,4,8
        self.dxy=[[0,-1],[1,0],[0,1],[-1,0]]
        self.dconidx=[1,2,4,8]
        """
        0,1,4,5,8,9,12,13,2,3,6,7,10,11,14,15
        """
        self.connectpattern={0:0,1:1,4:2,5:3,8:4,9:5,12:6,13:7,2:8
                    ,3:9,6:10,7:11,10:12,11:13,14:14,15:15}
    def PutPuyoImage(self,bgimage,puyoimage,r,c):
        w = bgimage.size[0]/10
        h = bgimage.size[1]/14
        bgimage.paste(puyoimage,(w*r,c*h),puyoimage)
    def makeField(self,fielddata,nexts):
        #make image
        tmp = self.fieldimg.copy()
        y=0
        w=tmp.size[0]/10
        h=tmp.size[1]/14
        #paste nexts
        if(nexts[0][0] != 'X'): self.PutPuyoImage(tmp,self.colorpuyo[self.colortoint[nexts[0][0]]][0],8,1)
        if(nexts[0][1] != 'X'): self.PutPuyoImage(tmp,self.colorpuyo[self.colortoint[nexts[0][1]]][0],8,2)
        if(nexts[1][0] != 'X'): self.PutPuyoImage(tmp,self.colorpuyo[self.colortoint[nexts[1][0]]][0],8,4)
        if(nexts[1][1] != 'X'): self.PutPuyoImage(tmp,self.colorpuyo[self.colortoint[nexts[1][1]]][0],8,5)
        curh=0
        while y<len(fielddata):
            curw=w
            x=0
            while x<len(fielddata[0]):
                if fielddata[y][x]!='.'  and fielddata[y][x]!='O':
                    colidx = self.colortoint[fielddata[y][x]]
                    conidx = 0
                    t = 0
                    for xy in self.dxy:
                        nx = x+xy[0]
                        ny = y+xy[1]
                        if 0<=nx and nx<len(fielddata[0]) and 0<=ny and ny<len(fielddata) :
                            if fielddata[y][x] == fielddata[ny][nx] :
                                conidx += self.dconidx[t]
                        t+=1
                    tmp.paste(self.colorpuyo[colidx][self.connectpattern[conidx]],(curw,curh),self.colorpuyo[colidx][self.connectpattern[conidx]])
                if fielddata[y][x] == 'O':
                    colidx = self.colortoint[fielddata[y][x]]
                    conidx = 0
                    tmp.paste(self.colorpuyo[colidx][self.connectpattern[conidx]],(curw,curh),self.colorpuyo[colidx][self.connectpattern[conidx]])
                curw+=w
                x+=1
            curh+=h
            y+=1
        return tmp
################################################################

#input
#fielddata = []
#for s in sys.stdin:
#    fielddata.append(s.strip())
argvs = sys.argv
argc = len(argvs)
if(argc !=2):
    print 'Usage: # python %s filename' % argvs[0]
    quit()
filename = argvs[1]
f = open(filename,'r')
PFM=PuyoFieldMaker()
os.mkdir(filename+"directory")
cnt=1
while 1:
    fielddata = []
    s=f.readline()
    if s[0]=='s' :
        break
    nexts = [];
    nexts.append(s[0]+s[1]);
    nexts.append(s[3]+s[4]);
    s = f.readline()
    fielddata.append(s.strip())
    r=1
    while r<13:
        s=f.readline()
        fielddata.append(s.strip())
        r+=1
    im=PFM.makeField(fielddata,nexts)
    im.save(filename+"directory/"+filename+str(cnt)+".png")
    f.readline()
    cnt+=1
