#include <QWidget>
#include "groupBox.h"

GroupBox::GroupBox(const QString &title, QWidget *parent ) {
	this->setTitle(title);
	this->setStyleSheet("QGroupBox { padding: 10px;  margin-top: 5px; border: 1px solid gray; } QGroupBox::title { subcontrol-origin: margin; top: -3px; left: 20px; subcontrol-position: top left; } ");
}

