#include "enni.h"

int main(int argc, char** argv)
{
	//---------------------------------------------------------------------------------
	// Inicializar la interfaz del programa
	PyImport_AppendInittab("enni", &PyInit_enni);
	//---------------------------------------------------------------------------------
	// Simulador Fisica (Bullet)
	world = bullet::PhysicsWorld::create();
	world->paused(true);
	//---------------------------------------------------------------------------------
	
	// Inicializar maquina de python
	Py_Initialize();
	FILE* file;
	file = fopen("./asset/config/init.py", "r");
	PyRun_SimpleFile(file, "./config/init.py");
	fclose(file);
	
	// Input
	//auto c = canvas->keyboard()->keyDown()->connect(keyboard_keyDown);
	
	// Canvas Run
	canvas->run();

	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}