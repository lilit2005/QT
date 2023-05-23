#include "configurator.h"
#include "bluePart.h"
#include "groupBox.h"
#include <iostream>

// Constructor for Configurator class
Configurator::Configurator() {
    m_pLayoutMain = new QGridLayout(this);
	this->setLayout(m_pLayoutMain);
	
	drawBluePart();

	m_jFile = new QFile(fileName);
	
	QTabWidget* pTabWidgetMainTab = new QTabWidget();
    m_pLayoutMain->addWidget(pTabWidgetMainTab);
	QWidget* pWidgetTabCentral = new QWidget();
	m_pLayoutTabCentralLayout = new QGridLayout(pWidgetTabCentral);
	pTabWidgetMainTab->addTab(pWidgetTabCentral, "Configuration");

	drawDataWidthConfiguration();
	drawAddressConfiguration();
	drawMemorySpace();
	drawAPBSlaveSlots();
	drawTestbench();
	drawLicense();
	drawButtons();

	insertValuesFromJson();

	for (int i = 0; i < m_pListMemorySpace.size(); i++) {
		connect(m_pListMemorySpace[i], &QCheckBox::stateChanged, this, [=]() {
			if (m_pListMemorySpace[i]->isChecked()) {
				m_pListSlaveSlots[i]->setEnabled(false);
			} else {
				m_pListSlaveSlots[i]->setEnabled(true);
			}
    	});
	}
	
	
	//connect( m_pList[i], signalName( int ), receiveri[i], slotOfReceiver( int ) )
	//class for checkboxes included slotOfReceiver function
	//if statement for checking slotOfReceiver argument
	/*	void signalName( int state )
		{
			receiveri[i]->slotOfReceiver( state );
		}*/
	//emit signalName( Qt::Unchecked )
	//receiver->slotOfReceiver( Qt::Unchecked )

	connect(m_pButtonCancel, &QPushButton::clicked, this, []() {QApplication::exit();});
	connect(m_pButtonConfirm, &QPushButton::clicked, this, [=](){createJsonFile();});
	connect(m_pComboBoxOfMasterAddress, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
    if (index == 0) {
        m_pComboBoxOfSlaveAddress->setEnabled(false);
		m_pComboBoxOfIndirectAddressing->setEnabled(false);
    } else {
        m_pComboBoxOfSlaveAddress->setEnabled(true);
		m_pComboBoxOfIndirectAddressing->setEnabled(true);
    }
});

	this->show();                                                                                                             
}                                                                                                                             

// function for BluePart object
void Configurator::drawBluePart() {
	BluePart* pBluePartTitle = new BluePart();
    m_pLayoutMain->addWidget(pBluePartTitle);
};

// First group box | Data Width Configuration
void Configurator::drawDataWidthConfiguration() {
	GroupBox* innerGroupBoxFirst = new GroupBox("Data Width Configuration", this);
    m_pLayoutTabCentralLayout->addWidget(innerGroupBoxFirst);
    QHBoxLayout* firstGroupBoxLayout = new QHBoxLayout(innerGroupBoxFirst);
    firstGroupBoxLayout->setSpacing(20);

    QLabel* radioButtonText = new QLabel("APB Master Data Bus Width", innerGroupBoxFirst);
    m_pRadioButtonFirst = new QRadioButton("32-bit", innerGroupBoxFirst);
    m_pRadioButtonSecond = new QRadioButton("16-bit", innerGroupBoxFirst);
    m_pRadioButtonThird = new QRadioButton("8-bit", innerGroupBoxFirst);

    firstGroupBoxLayout->addWidget(radioButtonText);
    firstGroupBoxLayout->addWidget(m_pRadioButtonFirst);
    firstGroupBoxLayout->addWidget(m_pRadioButtonSecond);
    firstGroupBoxLayout->addWidget(m_pRadioButtonThird);
    firstGroupBoxLayout->addStretch(1);
}


// Second group box | Address Configuration
void Configurator::drawAddressConfiguration() {
	GroupBox* innerGroupBoxSecond = new GroupBox("Address Configuration", this);
    m_pLayoutTabCentralLayout->addWidget(innerGroupBoxSecond);
    QGridLayout* secondGroupBoxLayout = new QGridLayout(innerGroupBoxSecond);
    secondGroupBoxLayout->setHorizontalSpacing(20);

    QLabel* masterAddress = new QLabel("Number of address bits driven by master:", innerGroupBoxSecond);
    m_pComboBoxOfMasterAddress = new QComboBox(innerGroupBoxSecond);
    m_pComboBoxOfMasterAddress->addItem("32");
    m_pComboBoxOfMasterAddress->addItem("28");
    m_pComboBoxOfMasterAddress->addItem("24");
    m_pComboBoxOfMasterAddress->addItem("20");
    m_pComboBoxOfMasterAddress->addItem("16");
    m_pComboBoxOfMasterAddress->addItem("12");

    QLabel* slaveAddress= new QLabel("Position in slave address of upper 4 bits of master address:", innerGroupBoxSecond);
    m_pComboBoxOfSlaveAddress = new QComboBox(innerGroupBoxSecond);
    m_pComboBoxOfSlaveAddress->addItem("Upper 4 master address bits do not appear in slave address [31:28]");
    m_pComboBoxOfSlaveAddress->addItem("[27:24](Ignored if master address width >= 32 bits)");
    m_pComboBoxOfSlaveAddress->addItem("[23:20](Ignored if master address width >= 28 bits)");
    m_pComboBoxOfSlaveAddress->addItem("[19:16](Ignored if master address width >= 24 bits)");
    m_pComboBoxOfSlaveAddress->addItem("[15:12](Ignored if master address width >= 20 bits)");
    m_pComboBoxOfSlaveAddress->addItem("[11:8](Ignored if master address width >= 16 bits)");

    QLabel* indirectAddressing = new QLabel("Indirect Addressing:", innerGroupBoxSecond);
    m_pComboBoxOfIndirectAddressing = new QComboBox(innerGroupBoxSecond);
    m_pComboBoxOfIndirectAddressing->addItem("Not in use");
    m_pComboBoxOfIndirectAddressing->addItem("Indirect address sourced from IADDR input port");
    m_pComboBoxOfIndirectAddressing->addItem("Indirect address sourced from register(s) in slot 0 space");

	addItemsInIndirectAddressing();

	QSpacerItem *secondSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    secondGroupBoxLayout->addItem(secondSpacer, 0, 2);
    
    secondGroupBoxLayout->addWidget(masterAddress, 0, 0);
    secondGroupBoxLayout->addWidget(m_pComboBoxOfMasterAddress, 0, 1);
    secondGroupBoxLayout->addWidget(slaveAddress, 1, 0);
    secondGroupBoxLayout->addWidget(m_pComboBoxOfSlaveAddress, 1, 1);
    secondGroupBoxLayout->addWidget(indirectAddressing, 2, 0);
    secondGroupBoxLayout->addWidget(m_pComboBoxOfIndirectAddressing, 2, 1);
}


// Third group box | Allocate memory space to combined region slave
void Configurator::drawMemorySpace() {
	GroupBox* innerGroupBoxThird = new GroupBox("Allocate memory space to combined region slave", this);
    m_pLayoutTabCentralLayout->addWidget(innerGroupBoxThird);
    QGridLayout* thirdGroupBoxLayout = new QGridLayout(innerGroupBoxThird);
    thirdGroupBoxLayout->setHorizontalSpacing(20);

    for (int a = 0; a < ROWS; a++) {
            for (int b = 0; b < COLUMNS; b++) {
                    int number = a*COLUMNS+b;
                    QString name = QString("Slot %1").arg(number);
                    QCheckBox* checkbox = new QCheckBox(name);
                    thirdGroupBoxLayout->addWidget(checkbox, a, b);
                    m_pListMemorySpace.append(checkbox);
            }
    }

    QSpacerItem *thirdSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    thirdGroupBoxLayout->addItem(thirdSpacer, 0, 4);
}


// Fourth group box | Enabled APB Slave Slots
void Configurator::drawAPBSlaveSlots() { 
	GroupBox* innerGroupBoxFourth = new GroupBox("Enabled APB Slave Slots", this);
    m_pLayoutTabCentralLayout->addWidget(innerGroupBoxFourth);
    QGridLayout* fourthGroupBoxLayout = new QGridLayout(innerGroupBoxFourth);
    fourthGroupBoxLayout->setHorizontalSpacing(20);

    for (int a = 0; a < ROWS; a++) {
        for (int b = 0; b < COLUMNS; b++) {
            int number = a*COLUMNS+b;
            QString name = QString("Slot %1").arg(number);
            QCheckBox* checkbox = new QCheckBox(name);
            fourthGroupBoxLayout->addWidget(checkbox, a, b);
            m_pListSlaveSlots.append(checkbox);
        }
    }

    QSpacerItem *fourthSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    fourthGroupBoxLayout->addItem(fourthSpacer, 0, 4);
}


// Testbench
void Configurator::drawTestbench() {
    QWidget* testbenchPortion = new QWidget();
    m_pLayoutTabCentralLayout->addWidget(testbenchPortion);
    QGridLayout* testbenchLayout = new QGridLayout(testbenchPortion);

    QLabel* testbenchTitle = new QLabel("Testbench:", testbenchPortion);
   	m_pComboBoxTestbench = new QComboBox(testbenchPortion);
    m_pComboBoxTestbench->addItem("None");
    m_pComboBoxTestbench->addItem("User");
    testbenchLayout->addWidget(testbenchTitle, 0, 0);
    testbenchLayout->addWidget(m_pComboBoxTestbench, 0, 1);
    testbenchLayout->setSpacing(10);

    QSpacerItem *testbenchSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    testbenchLayout->addItem(testbenchSpacer, 0, 3);
}


// License
void Configurator::drawLicense() {
    QWidget* licensePortion = new QWidget();
    m_pLayoutTabCentralLayout->addWidget(licensePortion);
    QGridLayout* licenseLayout = new QGridLayout(licensePortion);
    QLabel* licenseTitle = new QLabel("License:", licensePortion);
	m_pRadioButtonObfuscated = new QRadioButton("Obfuscated", licensePortion);
	m_pRadioButtonRTL = new QRadioButton("RTL", licensePortion);

    licenseLayout->addWidget(licenseTitle, 0, 0);
    licenseLayout->addWidget(m_pRadioButtonObfuscated, 0, 1);
    licenseLayout->addWidget(m_pRadioButtonRTL, 0, 2);
    licenseLayout->setSpacing(30);

    QSpacerItem *licenseSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    licenseLayout->addItem(licenseSpacer, 0, 3);
}


// Help, confirm, cancel buttons
void Configurator::drawButtons() {

	QDialogButtonBox* dbGroup = new QDialogButtonBox(this);
	m_pLayoutMain->addWidget(dbGroup);

	m_pButtonHelp = new QToolButton();
    m_pButtonHelp->setText("Help");
    m_pButtonHelp->setStyleSheet("width: 90%");
    QMenu *menu = new QMenu();
    menu->addAction("CoreAPB3_HB.pdf");
    menu->addAction("CoreAPB3_RN.pdf");
    m_pButtonHelp->setMenu(menu);
    m_pButtonHelp->setPopupMode(QToolButton::InstantPopup);

	m_pButtonConfirm = new QPushButton("OK", dbGroup);
    m_pButtonCancel = new QPushButton("Cancel", dbGroup);

	dbGroup->addButton(m_pButtonCancel, QDialogButtonBox::ActionRole);	
	dbGroup->addButton(m_pButtonConfirm, QDialogButtonBox::ActionRole);	
	dbGroup->addButton(m_pButtonHelp, QDialogButtonBox::HelpRole);
}

// function to add items in Indirect addressing
void Configurator::addItemsInIndirectAddressing() {
	for (int i = 1; i < 8; i++) {
		QString text = QString("Indirect address sourced from register(s) in slot %1 space").arg(i);
		m_pComboBoxOfIndirectAddressing->addItem(text);
	}
}


void Configurator::sendElementsValue() {
	// Data Width Configuration
	if (m_pRadioButtonFirst->isChecked()) {
		m_radioButton = 32;
	} else if (m_pRadioButtonSecond->isChecked()) {
		m_radioButton = 16;
	} else if (m_pRadioButtonThird->isChecked()) {
		m_radioButton = 8;
	}
	
	
	// Address configuration
	m_stringMasterAddressValue = m_pComboBoxOfMasterAddress->currentText();
	m_stringSlaveAddressValue = m_pComboBoxOfSlaveAddress->currentText();
	m_stringIndirectAddressingValue = m_pComboBoxOfIndirectAddressing->currentText();
	

	//Memory space
	int i = 0;
	for (auto *each : m_pListMemorySpace) {
	    QJsonObject jsonObjectMemorySpace;
	    jsonObjectMemorySpace[QString::number(i)] = false;
	    if (each->isChecked()) {
	        jsonObjectMemorySpace[QString::number(i)] = true;
	    }
	    m_jsonArrayMemorySpace.append(jsonObjectMemorySpace);
	    i++;
	}
	
	
	//APB Slave Slots
	int a = 0;
	for (auto *each : m_pListSlaveSlots) {
	    QJsonObject jsonObjectSlaveSlots;
	    jsonObjectSlaveSlots[QString::number(a)] = false;
	    if (each->isChecked()) {
	        jsonObjectSlaveSlots[QString::number(a)] = true;
	    }
	    m_jsonArraySlaveSlots.append(jsonObjectSlaveSlots);
	    a++;
	}
	

	//Testbench
	m_strTestBench = m_pComboBoxTestbench->currentText();
	
	
	//License
	if (m_pRadioButtonObfuscated->isChecked()) {
		m_strRadioButton = "Obfuscated";
	} else if (m_pRadioButtonRTL->isChecked()) {
		m_strRadioButton = "RTL";
	}
}


void Configurator::createJsonFile() {
	sendElementsValue();

    QFile m_jFile("data.json");
    if (!m_jFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open file.");
        return;
    }

	m_jsonObjectStructure["Data_Bus_Width"] = m_radioButton;
	m_jsonObjectStructure["Master_address"] = m_stringMasterAddressValue;
	m_jsonObjectStructure["Slave_address"] = m_stringSlaveAddressValue;
	m_jsonObjectStructure["Indirect_addressing"] = m_stringIndirectAddressingValue;
	m_jsonObjectStructure["Memory_space"] = m_jsonArrayMemorySpace;
	m_jsonObjectStructure["Slave_slots"] = m_jsonArraySlaveSlots;
	m_jsonObjectStructure["Testbench"] = m_strTestBench;
	m_jsonObjectStructure["License"] = m_strRadioButton;


	//Transfer data to json
	QJsonDocument document(m_jsonObjectStructure);
    m_jFile.write(document.toJson());
    m_jFile.close();

    //Exit from application
    QApplication::exit();
}


void Configurator::insertValuesFromJson() {
	readJsonFile();
	if(!m_jsReadJsonObject.isEmpty()) {
		// Data Bus Width Configuration
		if (m_jsReadJsonObject.value("Data_Bus_Width").toInt() == 32) {
			m_pRadioButtonFirst->setChecked(true);	
		} else if (m_jsReadJsonObject.value("Data_Bus_Width").toInt() == 16) {
			m_pRadioButtonSecond->setChecked(true);
		} else if (m_jsReadJsonObject.value("Data_Bus_Width").toInt() == 8) {
	        m_pRadioButtonThird->setChecked(true);
		} else if (m_jsReadJsonObject.value("Data_Bus_Width").toInt() == 0) {
            m_pRadioButtonFirst->setChecked(false);
            m_pRadioButtonSecond->setChecked(false);
            m_pRadioButtonThird->setChecked(false);
		}

		// Master address
		if (m_jsReadJsonObject.value("Master_address").toString() == "32") {
			m_pComboBoxOfMasterAddress->setCurrentText("32");
			m_pComboBoxOfSlaveAddress->setEnabled(false);
			m_pComboBoxOfIndirectAddressing->setEnabled(false);
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "28") {
			m_pComboBoxOfMasterAddress->setCurrentText("28");
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "24") {
            m_pComboBoxOfMasterAddress->setCurrentText("24");
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "20") {
            m_pComboBoxOfMasterAddress->setCurrentText("20");
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "16") {
            m_pComboBoxOfMasterAddress->setCurrentText("16");
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "12") {
            m_pComboBoxOfMasterAddress->setCurrentText("12");
		} else if (m_jsReadJsonObject.value("Master_address").toString() == "") {
            m_pComboBoxOfMasterAddress->setCurrentText("");
		}

		// Slave address
		if (m_jsReadJsonObject.value("Slave_address").toString() == "Upper 4 master address bits do not appear in slave address [31:28]") {
			m_pComboBoxOfSlaveAddress->setCurrentText("Upper 4 master address bits do not appear in slave address [31:28]");
		} else if (m_jsReadJsonObject.value("Slave_address").toString() == "[27:24](Ignored if master address width >= 32 bits)") {
			m_pComboBoxOfSlaveAddress->setCurrentText("[27:24](Ignored if master address width >= 32 bits)");
		} else if (m_jsReadJsonObject.value("Slave_address").toString() == "[23:20](Ignored if master address width >= 28 bits)") {
            m_pComboBoxOfSlaveAddress->setCurrentText("[23:20](Ignored if master address width >= 28 bits)");
        } else if (m_jsReadJsonObject.value("Slave_address").toString() == "[19:16](Ignored if master address width >= 24 bits)") {
            m_pComboBoxOfSlaveAddress->setCurrentText("[19:16](Ignored if master address width >= 24 bits)");
        } else if (m_jsReadJsonObject.value("Slave_address").toString() == "[15:12](Ignored if master address width >= 20 bits)") {
            m_pComboBoxOfSlaveAddress->setCurrentText("[15:12](Ignored if master address width >= 20 bits)");
        } else if (m_jsReadJsonObject.value("Slave_address").toString() == "[11:8](Ignored if master address width >= 16 bits)") {
            m_pComboBoxOfSlaveAddress->setCurrentText("[11:8](Ignored if master address width >= 16 bits)");
        }

		//Indirect addressing
		if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Not in use") {
			m_pComboBoxOfIndirectAddressing->setCurrentText("Not in use");
		} else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from IADDR input port") {
			m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from IADDR input port");
		} else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 0 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 0 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 1 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 1 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 2 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 2 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 3 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 3 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 4 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 4 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 5 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 5 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 6 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 6 space");
        } else if (m_jsReadJsonObject.value("Indirect_addressing").toString() == "Indirect address sourced from register(s) in slot 7 space") {
            m_pComboBoxOfIndirectAddressing->setCurrentText("Indirect address sourced from register(s) in slot 7 space");
        }


		// Memory space
		QJsonArray memorySpaceArray = m_jsReadJsonObject.value("Memory_space").toArray();
        for (int i = 0; i < memorySpaceArray.size(); i++) {
            QJsonObject innerJsonObject = memorySpaceArray.at(i).toObject();
            if (innerJsonObject.value(QString::number(i)) == false) {
                m_pListMemorySpace[i]->setChecked(false);
            } else if (innerJsonObject.value(QString::number(i)) == true) {
                m_pListMemorySpace[i]->setChecked(true);
				m_pListSlaveSlots[i]->setEnabled(false);
            }
        }


		//Slave slots
		QJsonArray slaveSlotsArray = m_jsReadJsonObject.value("Slave_slots").toArray();
        for (int i = 0; i < slaveSlotsArray.size(); i++) {
            QJsonObject innerJsonObject = slaveSlotsArray.at(i).toObject();
            if (innerJsonObject.value(QString::number(i)) == false) {
                m_pListSlaveSlots[i]->setChecked(false);
            } else if (innerJsonObject.value(QString::number(i)) == true) {
                m_pListSlaveSlots[i]->setChecked(true);
            }
        }


		// Testbench
		if (m_jsReadJsonObject.value("Testbench").toString() == "User") {
			m_pComboBoxTestbench->setCurrentText("User");	
		} else if (m_jsReadJsonObject.value("Testbench").toString() == "None") {
			m_pComboBoxTestbench->setCurrentText("None");
		}

		// License
		if (m_jsReadJsonObject.value("License").toString() == "Obfuscated") {
			m_pRadioButtonObfuscated->setChecked(true);
		} else if (m_jsReadJsonObject.value("License").toString() == "RTL") {
			m_pRadioButtonRTL->setChecked(true);
		} else if (m_jsReadJsonObject.value("License").toString() == "") {
			m_pRadioButtonRTL->setChecked(false);
			m_pRadioButtonObfuscated->setChecked(false);
		}
	}
}


//Function to read from json file
void Configurator::readJsonFile() {
    m_jFile->open(QIODevice::ReadOnly);
    QJsonDocument doc = QJsonDocument::fromJson(m_jFile->readAll());
	m_jsReadJsonObject = doc.object();
	m_jFile->close();
}

