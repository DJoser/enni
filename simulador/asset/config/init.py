from utils import *
import enni

nodo = enni.Node("nodo",[])

## Inicializar todas las Apis internas de ENNI
# Nota: Tienen que ser inicializadas en este orden para evitar 
#       para evitar errores de referencias 
s = enni.Scene()
i = enni.Input()
a = enni.Assets()
h = enni.Html()
p = enni.Physics()

# Cargar la interfaz grafica
h.loadPage('html/interface.html')

# Cargar los efectos 
a.queue("effect/Line.effect")
a.queue("effect/Phong.effect")
a.queue("effect/Basic.effect")
a.load()

# Crear escena basica
camera = s.createNode("camera")
s.nodeAddCamera()
#camera.modules.add(Camera(5.0f, 1.5f, 5.0f))

light = s.createNode("light")
s.nodeAddDirectionaLight()
##light.modules.add(directionalLigth(0.f, 2.f, 5.f))

# Buscar los modulos registrados
#########
robot = "asset/robot/Tyson/tyson.dae" 
a.queue(robot)
a.load()

cubo = s.createNode("cube")
s.nodeAddCube()
##cube.modules.add(geometry("cube"))

#########

# Ciclo del programa
while True:
    s.nextFrame()
