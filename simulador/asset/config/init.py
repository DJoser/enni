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
camera = "camera"
s.createNode(camera)
s.nodeAddCamera(camera)
s.nodeTransformLookAt(camera,5.0, 1.5, 5.0)

light = "light"
s.createNode(light)
s.nodeAddDirectionaLight(light)
s.nodeTransformLookAt(light,0.0, 2.0, 5.0)

# Buscar los modulos registrados
######## Modulo
robot = "robot/Tyson/model/tyson.dae" 
a.queue(robot)
a.load()

tyson = "tyson"
s.createNode(tyson)
s.nodeAddModel(tyson,robot)
########
