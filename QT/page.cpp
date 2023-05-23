#include "bluePart.h"
#include "groupBox.h"
#include "configurator.h"
#include <QWidget>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
    QMainWindow window;
    window.setWindowTitle("Configurator");
	Configurator object;
    return app.exec();	
}

