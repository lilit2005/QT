#include <QWidget>
#include <QGroupBox>

class GroupBox : public QGroupBox {
    public:
		GroupBox(const QString &title, QWidget *parent = nullptr); 
};

