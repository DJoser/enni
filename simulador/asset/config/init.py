import enni

# Inicializar todas las Apis internas de ENNI
a = enni.Assets()
h = enni.Html()
i = enni.Input()
p = enni.Physics()
s = enni.Scene()

print("Inicializando codigo")

h.loadPage('html/interface.html')
################################################################
# Agregar todos los recursos a cargar
a.queue("effect/Line.effect")
a.queue("effect/Phong.effect")
a.queue("effect/Basic.effect")
robot = "asset/robot/Tyson/tyson.dae" 
a.queue(robot)
a.load()
################################################################

#### Crear la escena
##s.createNode("luces")
##s["luces"].addLigth()

#print(s.createNode("Camara"))
##s["Camara"].addCamera()


##s.createNode("tyson")
##s["tyson"].addModel(robot)
##s["tyson"].addCollider("cube",0.5,0.5,0.5)
##s["tyson"].addPhysicsDebug()

##s.createNode("piso")
##s["piso"].addPlane(0.,0.,0.)
##s["piso"].addCollider("plane",0.,0.,0.)
##s["piso"].addPhysicsDebug()