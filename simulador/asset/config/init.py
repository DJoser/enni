import enni
help(enni.Node)

nodo = enni.Node("nodo",[])

## Inicializar todas las Apis internas de ENNI
# Nota: Tienen que ser inicializadas en este orden para evitar 
#       para evitar errores de referencias 
s = enni.Scene()
i = enni.Input()
a = enni.Assets()
h = enni.Html()
p = enni.Physics()

#print("Inicializando codigo")

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
camera = s.createNode("camera")
s.nodeAddCamera()
#camera.modules.add(Camera(5.0f, 1.5f, 5.0f))

light = s.createNode("light")
s.nodeAddDirectionaLight()
##light.modules.add(directionalLigth(0.f, 2.f, 5.f))

cubo = s.createNode("cube")
s.nodeAddCube()
##cube.modules.add(geometry("cube"))

tyson = s.createNode("tyson")
#s.nodeCreateModel()
##s["tyson"].addModel(robot)
##s["tyson"].addCollider("cube",0.5,0.5,0.5)
##s["tyson"].addPhysicsDebug()

#piso = s.createNode("piso")
##s["piso"].addPlane(0.,0.,0.)
##s["piso"].addCollider("plane",0.,0.,0.)
##s["piso"].addPhysicsDebug()

##class robot:
##    def __init__(self, *args, **kwargs):
##        brazo = serial.begin("COM1")
##        altura = serial.begin("COM2")
##        base = serial.begin("COM3")
##        return super().__init__(*args, **kwargs)

##    def rotarBrazo():
##        if simulado:
##            tyson.rotate()
##        else:
##            serial.print("")
##    def calibrar():
##        pass
##t = robot()
##t.rotarBrazo()


