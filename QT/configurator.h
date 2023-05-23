#include <QWidget>
#include <QGridLayout>
#include <QDialog>
#include <QTabWidget>
#include <QRadioButton>
#include <QComboBox>
#include <QList>
#include <QCheckBox>
#include <QToolButton>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class Configurator : public QDialog {

public:
    QGridLayout* m_pLayoutMain; 
    QGridLayout* m_pLayoutTabCentralLayout;

	QRadioButton* m_pRadioButtonFirst;
	QRadioButton* m_pRadioButtonSecond;
	QRadioButton* m_pRadioButtonThird;
	int m_radioButton = 0;
	
	QComboBox* m_pComboBoxOfMasterAddress;
	QComboBox* m_pComboBoxOfSlaveAddress;
	QComboBox* m_pComboBoxOfIndirectAddressing;
	QString m_stringMasterAddressValue;
	QString m_stringSlaveAddressValue;
	QString m_stringIndirectAddressingValue;
	
	QList<QCheckBox*> m_pListMemorySpace;
    QList<QCheckBox*> m_pListSlaveSlots;
	QJsonArray m_jsonArrayMemorySpace;
    QJsonArray m_jsonArraySlaveSlots;

	QComboBox* m_pComboBoxTestbench;
	QString m_strTestBench;

	QRadioButton* m_pRadioButtonObfuscated;
	QRadioButton* m_pRadioButtonRTL;
	QString m_strRadioButton;

	QToolButton* m_pButtonHelp;
	QPushButton* m_pButtonConfirm;
	QPushButton* m_pButtonCancel;

	QJsonObject m_jsonObjectStructure;
	QJsonObject m_jsReadJsonObject;
	QFile* m_jFile;
	const int ROWS = 4, COLUMNS = 4;
	const QString fileName = "data.json";
	QString m_license;

	Configurator();
	void drawBluePart();
	void drawDataWidthConfiguration();
	void drawAddressConfiguration();
	void drawMemorySpace();
	void drawAPBSlaveSlots();
	void drawTestbench(); 
	void drawLicense();  
	void drawButtons();
	void addItemsInIndirectAddressing();
	void createJsonFile();
	void sendElementsValue();
	void insertValuesFromJson();
	void readJsonFile();

};

