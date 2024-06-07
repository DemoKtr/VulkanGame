#include "app.h"
#include "cstdlib"
#include "windows.h"

int main() {
	
	App* myApp = new App(ivec2(640, 480), true);
	myApp->run();
	delete myApp;

	return 0;
}