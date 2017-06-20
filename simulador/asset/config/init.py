import enni

## Inicializar todas las Apis internas de ENNI
# Nota: Tienen que ser inicializadas en este orden para evitar 
#       para evitar errores de referencias 
s = enni.Scene()
i = enni.Input()
a = enni.Assets()
h = enni.Html()
p = enni.Physics()


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
camera = s.createNode("camera");
#node_add_camera(camera);
#node_transform_lookat(camera, math::vec3(5.f, 1.5f, 5.f));

light = s.createNode("ligth");
#node_add_directionalLigth(light);
#node_transform_lookat(light, math::vec3(0.f, 2.f, 5.f));

cubo = s.createNode("cube");
#node_add_cube(cubo);

tyson = s.createNode("tyson")
#s["tyson"].addModel(robot)
#s["tyson"].addCollider("cube",0.5,0.5,0.5)
#s["tyson"].addPhysicsDebug()

piso = s.createNode("piso")
#s["piso"].addPlane(0.,0.,0.)
#s["piso"].addCollider("plane",0.,0.,0.)
#s["piso"].addPhysicsDebug()