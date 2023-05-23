#include <QtWidgets>
#include <QLabel>
#include "bluePart.h"

BluePart::BluePart() {
	QFont font("Arial", 15, QFont::Bold);
	this->setFont(font);
	this->setText("<html><b><p>CoreAPB3 Configurator</p><p>Actel:DirectCore:CoreAPB3:4.2.100</p><b/></html>");
	this->setStyleSheet("padding: 20px; background-color: qlineargradient(x1: 0, y1: 1, x2: 1, y2: 1, stop: 0 #aacecf, stop: 0.75 #cee7ea, stop: 1 #d6e9ec);");
	this->show();
}

