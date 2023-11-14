import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import random
import vtkmodules.all as vtk

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
def creaCaminoCubos(n, VecCubos, num_cubo, NCubos):
    v_cubo_unit = np.array([
        [0, 0, 0],
        [1, 0, 0],
        [1, 1, 0],
        [0, 1, 0],
        [0, 0, 1],
        [1, 0, 1],
        [1, 1, 1],
        [0, 1, 1]])

    MatInfo = []

    for i in range(NCubos):
        Vec = []
        num_cubo += 1

        if (n == 1):
            Vec = [1, 3, 4, -3, -4]
        elif (n == 3):
            Vec = [3, 1, 4, -1, -4]
        elif (n == 4):
            Vec = [4, 3, 1, -3, -1]
        elif (n == -1):
            Vec = [-1, 3, 4, -3, -4]
        elif (n == -3):
            Vec = [-3, 1, 4, -1, -4]
        elif (n == -4):
            Vec = [-4, 3, 1, -3, -1]

        n = CaraAleatoria(Vec)

        if (n > 0):
            # Actualiza las coordenadas del cubo existente en la posición actual
            VecCubos[num_cubo - 1][n] = VecCubos[num_cubo - 1][n] + v_cubo_unit[n]
            nuevo_vertice = VecCubos[num_cubo - 1][n]
        elif (n == -1):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [-1, 0, 0]
        elif (n == -3):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [0, -1, 0]
        elif (n == -4):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [0, 0, -1]

        MatInfo.append([num_cubo - 1, DictCaraI[n], num_cubo, DictCaraF[n]])

    return (VecCubos, MatInfo)

# Crea camino de NCubos cubos desde CuboIni
def creaCaminosCiegosCubos(n, VecCubos, num_cubo, NCubos):

    v_cubo_unit = np.array([
        [0, 0, 0],
        [1, 0, 0],
        [1, 1, 0],
        [0, 1, 0],
        [0, 0, 1],
        [1, 0, 1],
        [1, 1, 1],
        [0, 1, 1]])

    MatInfo = []

    for i in range(NCubos):
        Vec = []
        num_cubo += 1

        if (n == 1):
            Vec = [1, 3, 4, -3, -4]
        elif (n == 3):
            Vec = [3, 1, 4, -1, -4]
        elif (n == 4):
            Vec = [4, 3, 1, -3, -1]
        elif (n == -1):
            Vec = [-1, 3, 4, -3, -4]
        elif (n == -3):
            Vec = [-3, 1, 4, -1, -4]
        elif (n == -4):
            Vec = [-4, 3, 1, -3, -1]

        n = CaraAleatoria(Vec)

        if (n > 0):
            # Actualiza las coordenadas del cubo existente en la posición actual
            VecCubos[num_cubo - 1][n] = VecCubos[num_cubo - 1][n] + v_cubo_unit[n]
            nuevo_vertice = VecCubos[num_cubo - 1][n]
        elif (n == -1):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [-1, 0, 0]
        elif (n == -3):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [0, -1, 0]
        elif (n == -4):
            nuevo_vertice = VecCubos[num_cubo - 1][0] + [0, 0, -1]

        MatInfo.append([num_cubo - 1, DictCaraI[n], num_cubo, DictCaraF[n]])

        VecCubos[num_cubo - 1] = v_cubo_unit + nuevo_vertice  # actualiza el cubo existente

    return (VecCubos, MatInfo)


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

# Cantidad de niveles
num_niveles = 3

for nivel in range(num_niveles):
    # Crear el camino principal del laberinto (de entrada a salida)
    LongLab = 200
    VecCubos = []
    VecCubos.append(v_cubo_unit)

    VecCubos, MatInfo = creaCaminoCubos(3, VecCubos, 0, LongLab)
    M = np.array(MatInfo)

    # Crear un número de caminos ciegos del laberinto (de entrada a salida)
    n_caminos_ciegos = 30
    v_aleat = np.random.randint(0, LongLab, size=n_caminos_ciegos)

    VecCaminosCiegos = []
    MatInfoCiegos = []
    for i in range(0, n_caminos_ciegos):

        val = v_aleat[i]

        caraf = M[M[:, 0] == val, 1]
        carai = M[M[:, 2] == val, 3]

        CaraSalida = DictVertCara[CaraAleatoria2(caraf, carai)]

        VecCaminoCiegoi = []
        VecCaminoCiegoi.append(VecCubos[val])

        VecCaminoCiegoi, MatInfo2 = creaCaminosCiegosCubos(
            CaraSalida, VecCaminoCiegoi, len(VecCubos) + len(VecCaminosCiegos), 20)

        VecCaminosCiegos = VecCaminosCiegos + VecCaminoCiegoi
        MatInfoCiegos = MatInfoCiegos + MatInfo2

    # Almacenar la información final del laberinto actual
    MatFinal = MatInfo + MatInfoCiegos

    # Convertir a DataFrame y guardar en CSV
    M = np.array(MatFinal)
    base = pd.DataFrame(M)
    base.to_csv(f'Laberinto_{nivel + 1}.csv', index=False)

    # Configurar para el siguiente nivel
    v_cubo_unit += VecCubos[-1][caras[0]]

    """
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
    

#ax.set_xlim([-30, 30])
#ax.set_ylim([-30, 30])
#ax.set_zlim([-30, 30])

#plt.show()


MatFinal=MatInfo+MatInfoCiegos


M=np.array(MatFinal)

base= pd.DataFrame(M)

# Cargar el laberinto desde el archivo CSV (Base.csv)
#a = pd.read_csv('Base.csv')
"""
laberinto = np.array(base)

# Crear una representación visual del laberinto
def create_cube_actor(position, color):
    cube = vtk.vtkCubeSource()
    cube.SetCenter(position)
    cube.SetXLength(1.0)
    cube.SetYLength(1.0)
    cube.SetZLength(1.0)

    cube_mapper = vtk.vtkPolyDataMapper()
    cube_mapper.SetInputConnection(cube.GetOutputPort())

    cube_actor = vtk.vtkActor()
    cube_actor.SetMapper(cube_mapper)
    cube_actor.GetProperty().SetColor(color)

    return cube_actor

# Crear una ventana de renderizado
render_window = vtk.vtkRenderWindow()
render_window.SetWindowName("Recorrido en el Laberinto")

# Crear un interactor para la ventana de renderizado
interactor = vtk.vtkRenderWindowInteractor()
interactor.SetRenderWindow(render_window)

# Crear un renderizador
renderer = vtk.vtkRenderer()
render_window.AddRenderer(renderer)

# Colores para representar el inicio y la salida
color_inicio = [1.0, 0.0, 0.0]  # Rojo
color_salida = [0.0, 1.0, 0.0]  # Verde

# Coordenadas del cubo de inicio y salida (ajusta según tus datos)
coord_inicio = np.array([0.0, 1.0, 1.0])
coord_salida = np.array([1.0, 1.0, 1.0])

# Crear actores para el cubo de inicio y salida
actor_inicio = create_cube_actor(coord_inicio, color_inicio)
actor_salida = create_cube_actor(coord_salida, color_salida)

# Agregar los actores al renderizador
renderer.AddActor(actor_inicio)
renderer.AddActor(actor_salida)

# Crear un actor para el recorrido
color_recorrido = [0.0, 0.0, 1.0]  # Azul
actor_recorrido = vtk.vtkAssembly()

# Recorre el laberinto y crea actores para el recorrido
for cubo in laberinto:
    position = np.array([cubo[0] % 5, (cubo[0] // 5) % 5, cubo[0] // 25])
    actor_recorrido.AddPart(create_cube_actor(position, color_recorrido))

# Agregar el actor del recorrido al renderizador
renderer.AddActor(actor_recorrido)

# Configurar la cámara
renderer.GetActiveCamera().Azimuth(30)
renderer.GetActiveCamera().Elevation(30)
renderer.GetActiveCamera().Zoom(1.0)

# Iniciar la interacción
render_window.Render()
interactor.Start()

##### -------------------- opcion 1  -------------------------####
import pygame
from pygame.locals import *


# Configuración de Pygame
pygame.init()
clock = pygame.time.Clock()
screen = pygame.display.set_mode((800, 600))

# Configuración de la cámara
camera_position = [0.5, 0.5, 0.5]
camera_speed = 0.05

# Bucle principal de Pygame
running = True
while running:
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    keys = pygame.key.get_pressed()
    if keys[K_UP]:
        camera_position[1] += camera_speed
    if keys[K_DOWN]:
        camera_position[1] -= camera_speed
    if keys[K_LEFT]:
        camera_position[0] -= camera_speed
    if keys[K_RIGHT]:
        camera_position[0] += camera_speed

    # Actualizar la posición de la cámara en VTK
    renderer.GetActiveCamera().SetPosition(camera_position)

    # Renderizar la escena
    render_window.Render()

    # Controlar la velocidad de actualización
    clock.tick(60)

pygame.quit()
##### -------------------- opcion 2  -------------------------####
import pybullet as p
import vtkmodules.all as vtk
import pygame
from pygame.locals import *

# Inicializar Pybullet
p.connect(p.GUI)
p.setGravity(0, 0, -9.81)

# Crear un cubo en Bullet Physics
cube_id = p.createCollisionShape(p.GEOM_BOX, halfExtents=[1, 1, 1])
cube_body_id = p.createMultiBody(baseCollisionShapeIndex=cube_id, basePosition=[0, 0, 1])

# Configurar la cámara de VTK
renderer = vtk.vtkRenderer()
render_window = vtk.vtkRenderWindow()
render_window.SetWindowName("Bullet Physics Integration")
render_window.AddRenderer(renderer)
iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(render_window)

# Crear un actor para el cubo de Bullet Physics
cube_actor = vtk.vtkActor()
cube_mapper = vtk.vtkPolyDataMapper()
cube_mapper.SetInputConnection(vtk.vtkCubeSource().GetOutputPort())
cube_actor.SetMapper(cube_mapper)

# Agregar el actor al renderizador de VTK
renderer.AddActor(cube_actor)

# Configuración de Pygame
pygame.init()
clock = pygame.time.Clock()
screen = pygame.display.set_mode((800, 600))

# Configuración de la cámara simulada
camera_position = [0, 0, 5]
camera_speed = 0.1

# Bucle principal
running = True
while running:
    for event in pygame.event.get():
        if event.type == QUIT:
            running = False
        elif event.type == KEYDOWN:
            if event.key == K_ESCAPE:
                running = False

    keys = pygame.key.get_pressed()
    if keys[K_UP]:
        camera_position[1] += camera_speed
    if keys[K_DOWN]:
        camera_position[1] -= camera_speed
    if keys[K_LEFT]:
        camera_position[0] -= camera_speed
    if keys[K_RIGHT]:
        camera_position[0] += camera_speed

    # Actualizar la posición de la cámara en Bullet Physics
    p.resetBasePositionAndOrientation(cube_body_id, camera_position, [0, 0, 0, 1])

    # Actualizar la posición de la cámara en VTK
    renderer.GetActiveCamera().SetPosition(camera_position)

    # Renderizar la escena
    render_window.Render()

    # Controlar la velocidad de actualización
    clock.tick(60)

p.disconnect()
pygame.quit()