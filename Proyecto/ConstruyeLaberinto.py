# -*- coding: utf-8 -*-
"""
Created on Sun May 28 11:07:30 2023

@author: ricor
"""
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import random

#Cara Aleatoria durante la construccion del camino principal
def CaraAleatoria(Vec):
    
    r=random.uniform(0,1)
    
    if(r<0.05):
        n=Vec[1]
    elif(r>=0.05 and r<0.1):
        n=Vec[2]
    elif(r>=0.1 and r<0.15):
        n=Vec[3]
    elif(r>=0.15 and r<0.2):
        n=Vec[4]
    else:
        n=Vec[0]
    return n

#Cara Aleatoria durante la construccion de caminos ciegos
def CaraAleatoria2(c1,c2):
    
    Vec=[]
    for i in range(1,7):
        if (i!=c1 and i!=c2):
            Vec.append(i)
    
    r=random.uniform(0,1)
    
    if(r<0.25):
        n=Vec[0]
    elif(r>=0.25 and r<0.5):
        n=Vec[1]
    elif(r>=0.5 and r<0.75):
        n=Vec[2]
    else:
        n=Vec[3]
    return n

#Crea camino de NCubos cubos desde CuboIni
def creaCaminoCubos(n,VecCubos,num_cubo,NCubos):

    v_cubo_unit = np.array([
         [0, 0, 0],
         [1, 0, 0],
         [1, 1, 0],
         [0, 1, 0],
         [0, 0, 1],
         [1, 0, 1],
         [1, 1, 1],
         [0, 1, 1]])

    MatInfo=[]
    
    for i in range(NCubos):
    
        Vec=[]
        num_cubo+=1
        
        if (n==1):
            Vec=[1,3,4,-3,-4]
        elif (n==3):
            Vec=[3,1,4,-1,-4]
        elif (n==4):
            Vec=[4,3,1,-3,-1]
        elif (n==-1):
            Vec=[-1,3,4,-3,-4]
        elif (n==-3):
            Vec=[-3,1,4,-1,-4]
        elif (n==-4):
            Vec=[-4,3,1,-3,-1]
        
        n=CaraAleatoria(Vec)
    
        if (n>0):
            nuevo_vertice=VecCubos[-1][n]
        elif (n==-1):
            nuevo_vertice=VecCubos[-1][0]+[-1,0,0]
        elif (n==-3):
            nuevo_vertice=VecCubos[-1][0]+[0,-1,0]
        elif (n==-4):
            nuevo_vertice=VecCubos[-1][0]+[0,0,-1]
        
        MatInfo.append([num_cubo-1,DictCaraI[n],num_cubo,DictCaraF[n]])
            
        
        VecCubos.append(v_cubo_unit+nuevo_vertice)#adiciona un nuevo cubo en nuevo vertice
        
    return(VecCubos,MatInfo)

#Crea camino de NCubos cubos desde CuboIni
def creaCaminosCiegosCubos(n,VecCubos,num_cubo,NCubos):

    v_cubo_unit = np.array([
         [0, 0, 0],
         [1, 0, 0],
         [1, 1, 0],
         [0, 1, 0],
         [0, 0, 1],
         [1, 0, 1],
         [1, 1, 1],
         [0, 1, 1]])    

    MatInfo=[]
    
    for i in range(NCubos):
    
        Vec=[]
        num_cubo+=1
        
        if (n==1):
            Vec=[1,3,4,-3,-4]
        elif (n==3):
            Vec=[3,1,4,-1,-4]
        elif (n==4):
            Vec=[4,3,1,-3,-1]
        elif (n==-1):
            Vec=[-1,3,4,-3,-4]
        elif (n==-3):
            Vec=[-3,1,4,-1,-4]
        elif (n==-4):
            Vec=[-4,3,1,-3,-1]
        
        n=CaraAleatoria(Vec)
                 
        if (n>0):
            nuevo_vertice=VecCubos[-1][n]
        elif (n==-1):
            nuevo_vertice=VecCubos[-1][0]+[-1,0,0]
        elif (n==-3):
            nuevo_vertice=VecCubos[-1][0]+[0,-1,0]
        elif (n==-4):
            nuevo_vertice=VecCubos[-1][0]+[0,0,-1]
        
        MatInfo.append([num_cubo-1,DictCaraI[n],num_cubo,DictCaraF[n]])
            
        
        VecCubos.append(v_cubo_unit+nuevo_vertice)#adiciona un nuevo cubo en nuevo vertice
        
    return(VecCubos,MatInfo)


'''---------------------------------Main-------------------------------------------'''

#Defino parametros
DictCaraI={-3:1,1:2,3:3,-1:4,4:5,-4:6}
DictCaraF={-3:3,1:4,3:1,-1:2,4:6,-4:5}
DictVertCara={1:-3,2:1,3:3,4:-1,5:4,6:-4}

origin = [0, 0, 0]

v_cubo_unit = np.array([
     [0, 0, 0],
     [1, 0, 0],
     [1, 1, 0],
     [0, 1, 0],
     [0, 0, 1],
     [1, 0, 1],
     [1, 1, 1],
     [0, 1, 1]]) 

caras = np.array([
    [0, 1, 5, 4],
    [1, 2, 6, 5],
    [2, 3, 7, 6],
    [0, 3, 7, 4],
    [4, 5, 6, 7],
    [0, 1, 2, 3],])

#Creo el camino principal del laberinto (de entrada a salida)

LongLab=200
VecCubos=[]
VecCubos.append(v_cubo_unit)

VecCubos,MatInfo=creaCaminoCubos(3,VecCubos,0,LongLab)
M=np.array(MatInfo)

#Creo un número de caminos ciegos del laberinto (de entrada a salida)
n_caminos_ciegos=30
v_aleat = np.random.randint(0,LongLab, size=n_caminos_ciegos)

VecCaminosCiegos=[]
MatInfoCiegos=[]
for i in range(0,n_caminos_ciegos):
    
    val=v_aleat[i]
    
    caraf =M[M[:, 0] ==val, 1]
    carai =M[M[:, 2] ==val, 3]
           
    CaraSalida=DictVertCara[CaraAleatoria2(caraf,carai)]
    
    VecCaminoCiegoi=[]
    VecCaminoCiegoi.append(VecCubos[val])
    
    VecCaminoCiegoi,MatInfo2=creaCaminosCiegosCubos(CaraSalida,VecCaminoCiegoi,len(VecCubos)+len(VecCaminosCiegos),20)
    
    VecCaminosCiegos=VecCaminosCiegos+VecCaminoCiegoi
    MatInfoCiegos=MatInfoCiegos+MatInfo2
    

# Create a 3D figure
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
    
for i in range(len(VecCubos)):   
    cube = Poly3DCollection(VecCubos[i][caras])
    cube.set_edgecolor('k')
    cube.set_facecolor('red')
    ax.add_collection3d(cube)

for i in range(len(VecCaminosCiegos)):   
    cube = Poly3DCollection(VecCaminosCiegos[i][caras])
    cube.set_edgecolor('k')
    cube.set_facecolor('blue')
    ax.add_collection3d(cube)    
    

ax.set_xlim([-30, 30])
ax.set_ylim([-30, 30])
ax.set_zlim([-30, 30])

plt.show()


MatFinal=MatInfo+MatInfoCiegos


M=np.array(MatFinal)

base= pd.DataFrame(M)
base.to_csv(r'C:\Users\ricor\OneDrive\Desktop\Files To Run\BasePrueba2Lab.csv', index=False)






