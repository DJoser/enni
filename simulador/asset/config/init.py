import enni

# Inicializar todas las Apis internas de ENNI
a = enni.Assets()
h = enni.Html()
i = enni.Input()
p = enni.Physics()
s = enni.Scene()

# Script de configuracion inicial
h.loadPage('html/interface.html')

h.startLoad()
##############################################################
# Agregar todos los recursos a cargar
a.add("effect/Line.effect")
a.add("effect/Phong.effect")
a.add("effect/Basic.effect")
robot = "asset/robot/Tyson/tyson.dae" 
a.add(robot)
a.load()
##############################################################
h.endLoad()

## Crear la escena
s.createNode("luces")
s["luces"].addLigth()

s.createNode("Camara")
s["Camara"].addCamera()

s.createNode("tyson")
s["tyson"].addModel(robot)
s["tyson"].addCollider("cube",0.5,0.5,0.5)
s["tyson"].addPhysicsDebug()

s.createNode("piso")
s["piso"].addPlane(0.,0.,0.)
s["piso"].addCollider("plane",0.,0.,0.)
s["piso"].addPhysicsDebug()

