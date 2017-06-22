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
	PyRun_SimpleFile(file, "__start__");
	fclose(file);

	while (true)
	{
		canvas->step();
	}

	if(Py_FinalizeEx() < 0) exit(120);

	return EXIT_SUCCESS;
}