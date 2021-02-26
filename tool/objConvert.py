import struct
import sys
class ObjProcessor():
    def __init__(self,uvsize=64):
        self.uvSize = uvsize
        
    def ReadInObj(self,fileName):
        self.fileName = fileName
        self.v = []#output vector
        
        self.vn = []#normal
        self.vt =[]#uv
        self.fvIdx=[]#output face index
        self.fvtIdx=[]
        self.fvnIdx=[]
        self.uvOutput=[]#uv output directly
        self.normalOutput =[]#normal output directly
        with open(fileName,"r") as file:
            objDatalines = file.readlines()
        for line in objDatalines:
            line =line.strip()
            if (line[0]=="#"):
                continue
            line = line.split(" ")


            for element in line:
                if element =='':
                    line.remove(element)


            if line[0] == "v" : #a vect line
                #print(line[1])
                self.v.append(float(line[1]))
                self.v.append(float(line[2]))
                self.v.append(float(line[3]))
            if line[0] == "vn" :#normal vect for face
                #print(line[1])
                self.vn.append(float(line[1]))
                self.vn.append(float(line[2]))
                self.vn.append(float(line[3]))
            if line[0] == "vt":
                #print(line[1])
                mapu = int(round(float(line[1])*(self.uvSize-1)))
                mapv = self.uvSize-1 - int(round(float(line[2])*(self.uvSize-1)))
                if mapu<0:
                    mapu=0
                if mapu>=self.uvSize:
                    mapu=self.uvSize -1
                if mapv<0:
                    mapv=0
                if mapv>=self.uvSize:
                    mapv=self.uvSize -1    

                self.vt.append(mapu)
                self.vt.append(mapv)
        for line in objDatalines:
            line = line[:-1]
            line = line.split(" ")
            for element in line:
                if element =='':
                    line.remove(element)

            if line[0] == "f":

                #print(line)
                #v/vt/vn
                faceData0 = line[1].split("/")

                self.fvIdx.append(int(faceData0[0])-1)
                self.fvtIdx.append(int(faceData0[1])-1)
                self.fvnIdx.append(int(faceData0[2])-1)
                faceData1 = line[2].split("/")
                self.fvIdx.append(int(faceData1[0])-1)
                self.fvtIdx.append(int(faceData1[1])-1)
                #self.fvnIdx.append(int(faceData1[2])-1)
                faceData2 = line[3].split("/")
                self.fvIdx.append(int(faceData2[0])-1)
                self.fvtIdx.append(int(faceData2[1])-1)
                #self.fvnIdx.append(int(faceData2[2])-1)
        #create output array
        for uvIdx in self.fvtIdx:
            mapu = self.vt[(uvIdx)*2]
            mapv = self.vt[(uvIdx)*2+1]
            self.uvOutput.append(mapu)
            self.uvOutput.append(mapv)

        #if (self.fvnIdx[0]!=self.fvnIdx[1]):
        #    print("Not face normal!\n")
        for i in range(len(self.fvnIdx)):
            self.normalOutput.append(self.vn[(self.fvnIdx[i])*3])
            self.normalOutput.append(self.vn[(self.fvnIdx[i])*3+1])
            self.normalOutput.append(self.vn[(self.fvnIdx[i])*3+2])
            #self.normalOutput.append(self.vn[(self.fvnIdx[i*3])*3])
            #self.normalOutput.append(self.vn[(self.fvnIdx[i*3])*3+1])
            #self.normalOutput.append(self.vn[(self.fvnIdx[i*3])*3+2])
        #cal the bound box
        vx=[]
        vy=[]
        vz=[]
        for i in range(len(self.v)//3):
            vx.append(self.v[3*i])
            vy.append(self.v[3*i+1])
            vz.append(self.v[3*i+2])
        vx.sort()
        vy.sort()
        vz.sort()
        self.vxMin=vx[0]
        self.vxMax=vx[-1]
        self.vyMin = vy[0]
        self.vyMax= vy[-1]
        self.vzMin= vz[0]
        self.vzMax= vz[-1]
        self.bound = []
        self.bound.append(self.vxMax)
        self.bound.append(self.vyMax)
        self.bound.append(self.vzMax)
        self.bound.append(self.vxMin)
        self.bound.append(self.vyMin)
        self.bound.append(self.vzMin)
        self.vectNum = len(self.v)//3
        self.triNum = len(self.fvIdx)//3
        self.vnNum = len(self.vn)//3
        self.outputLenght = self.CalMeshArrayLength()
        
    def CalMeshArrayLength(self,output_uv=True,output_normal=True):
        if output_uv == True:
            num = 12 + 4*6 +self.vectNum*12+ self.triNum*6 + self.triNum*6 

        else:
            num = 12 + 4*6 +self.vectNum*12+ self.triNum*6 
            if (self.triNum & 0x01):
                num = num +2
        if output_normal == True:
            num = num + self.triNum*12
        num = num//4
        return num
        
    def GenerateOutput(self,nameId,numId,output_uv=True,output_normal = True):
        self.outputLenght = self.CalMeshArrayLength(output_uv,output_normal)
        with open(self.fileName[:-3]+"h",'w') as outHFile:
            outHFile.write("#ifndef __"+nameId+"_H__\n")
            outHFile.write("#define __"+nameId+"_H__\n")
            outHFile.write("#ifdef __cplusplus\n")
            outHFile.write("extern \"C\" {\n")
            outHFile.write("#endif\n")
            outHFile.write("#include \"b3d_types.h\"\n")
            outHFile.write("extern const B3L_Mesh_t "+nameId+"_Mesh_Tex["+str(self.outputLenght)+"];\n")
            outHFile.write("#ifdef __cplusplus\n")
            outHFile.write("}\n")
            outHFile.write("#endif\n")
            outHFile.write("#endif\n")
        with open(self.fileName[:-3]+"c",'w') as outFile:    
            outFile.write("#include \"b3d_types.h\"\n")
            outFile.write("const B3L_Mesh_t "+nameId+"_Mesh_Tex["+str(self.outputLenght)+"]={\n")
            #output the main data
            size = (self.triNum<<16)|(self.vectNum)
            outFile.write("0x%08X, "%(numId))
            outFile.write("0x%08X, "%(size))
            if output_uv == True:
                low = self.triNum
            else:
                low = 0 
            if output_normal == True:
                high = self.triNum
            else:
                high = 0     
                
            size2 = ((high<<16)|low)
            outFile.write("0x%08X,//U32 ID num H16:triNum L16:vectNum H16:normalNum L16:uvNum\n"%(size2))
#output vectors
            for i in range(6):
                data = struct.unpack("<I",struct.pack("<f",self.bound[i]))[0]
                outFile.write("0x%08X, "%(data))
            outFile.write("//bound box max_x, max_y, max_z, min_x, min_y, min_z\n")
            
            left = self.vectNum*3 - (((self.vectNum*3)>>3)<<3)
            #vect 
            for i in range((self.vectNum*3)//8):
                for j in range(8):
                    data = struct.unpack("<I",struct.pack("<f",self.v[i*8+j]))[0]
                    outFile.write("0x%08X, "%(data))
                if (i==0):
                    outFile.write("//vectors\n")
                else:
                    outFile.write("\n")
            for i in range(left):
                data = struct.unpack("<I",struct.pack("<f",self.v[(((self.vectNum*3)>>3)<<3)+i]))[0]
                outFile.write("0x%08X, "%(data))
            if (left != 0):
                outFile.write("\n")   
#output triIdx
            totalleft = self.triNum*3 - (((self.triNum*3)>>1)<<1)
            bodyNum = (((self.triNum*3)>>1)<<1)
            left = (bodyNum>>1) - (((bodyNum>>1)//8)*8)
            for i in range(((self.triNum*3)>>1)//8):
                for j in range(8):
                    low = self.fvIdx[i*8*2+2*j]
                    high = self.fvIdx[i*8*2+2*j+1]
                    data = ((high<<16)|low)
                    outFile.write("0x%08X, "%(data))
                if (i==0):
                    outFile.write("//Tri index\n")
                else:
                    outFile.write("\n")
            for i in range(left):
                low = self.fvIdx[((((self.triNum*3)//2))//8)*8*2+i*2]
                high = self.fvIdx[((((self.triNum*3)//2))//8)*8*2+i*2+1]
                data = ((high<<16)|low)
                outFile.write("0x%08X, "%(data))
                
            if (totalleft == 1):
                if output_uv == True:
                    middle = self.uvOutput[0]
                    high = self.uvOutput[1]
                    low = self.fvIdx[-1]
                    data = (high<<24)|(middle<<16)|low
                    outFile.write("0x%08X, //with 2 bytes from uv data\n"%(data))
                    uvdataShift = 2
                else:
                    low = self.fvIdx[-1]
                    outFile.write("0x%08X, //with 2 bytes dummy data\n"%(low))
            else:
                uvdataShift = 0
                outFile.write("\n")
#output uv    
            if output_uv == True:    
                uvNumUnit4 = (self.triNum*6)//4    

                left = uvNumUnit4 - (uvNumUnit4//8)*8

                for i in range(uvNumUnit4//8):
                    for j in range(8):
                        uv0 = self.uvOutput[i*8*4+j*4+uvdataShift]
                        uv1 = self.uvOutput[i*8*4+j*4+uvdataShift+1]
                        uv2 = self.uvOutput[i*8*4+j*4+uvdataShift+2]
                        uv3 = self.uvOutput[i*8*4+j*4+uvdataShift+3]
                        data = (uv3<<24)|(uv2<<16)|(uv1<<8)|uv0
                        outFile.write("0x%08X, "%(data))
                    if (i==0):
                        outFile.write("//uv\n")
                    else:
                        outFile.write("\n")
                for i in range(left):
                    uv0 = self.uvOutput[(uvNumUnit4//8)*8*4+uvdataShift+i*4]
                    uv1 = self.uvOutput[(uvNumUnit4//8)*8*4+uvdataShift+i*4+1]
                    uv2 = self.uvOutput[(uvNumUnit4//8)*8*4+uvdataShift+i*4+2]
                    uv3 = self.uvOutput[(uvNumUnit4//8)*8*4+uvdataShift+i*4+3]
                    data = (uv3<<24)|(uv2<<16)|(uv1<<8)|uv0
                    outFile.write("0x%08X, "%(data))
                
                if (left != 0):
                    outFile.write("\n")  
#output normal
            if output_normal == True:
                left = (self.triNum*3) - (((self.triNum*3)//8)*8)

                for i in range((self.triNum*3)//8):
                    for j in range(8):
                        data = struct.unpack("<I",struct.pack("<f",self.normalOutput[i*8+j]))[0]
                        outFile.write("0x%08X, "%(data))
                    if (i==0):
                        outFile.write("//Normal\n")
                    else:
                        outFile.write("\n")
                for i in range(left):
                    data = struct.unpack("<I",struct.pack("<f",self.normalOutput[((self.triNum*3)//8)*8+i]))[0]
                    outFile.write("0x%08X, "%(data))
                if (left != 0):
                    outFile.write("\n")
            outFile.write("};\n")

if __name__ == "__main__":

    for i, arg in enumerate(sys.argv):
        print(f"Argument {i:>6}: {arg}")
    fileName = sys.argv[1]
    objName = sys.argv[2]
    objID = int(sys.argv[3])
    
    uvline = sys.argv[4].split("=")
    uvSize = int(uvline[1])
    if uvSize == 0 :
        includeUV = False
        op=ObjProcessor()
    else:
        includeUV = True
        op=ObjProcessor(uvSize)
    if len(sys.argv)>=6:
        if sys.argv[5] == 'Normal':
            includeNormal = True
        else:
            includeNormal = False
    else:
        includeNormal = True
    
    
    op.ReadInObj(fileName)
    op.GenerateOutput(objName,objID,includeUV ,includeNormal)
    