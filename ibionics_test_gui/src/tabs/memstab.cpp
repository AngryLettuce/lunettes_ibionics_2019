#include "includes/tabs/memstab.h"


MemsTab::MemsTab(QWidget *parent) : QWidget(parent)
{
    //Layout
    QGridLayout *layout = new QGridLayout(this);

    //PixMap
    QString filename = "C:/Users/houma/Documents/ibionicslunettes/ibionics_test_gui/images/eye.jpg";
    lbl = new MediaLabel(this, filename);
    posMouseLabel = new QLabel(this);
    posMouseLabel->setText("X:0,Y0");

    //Sequence
    QLabel *seqLbl = new QLabel("Sequence: ",this);
    QComboBox *seqCombo = new QComboBox();
    seqCombo->addItem("Rectangle");
    seqCombo->addItem("Closing Rectangle");
    seqCombo->addItem("Spiral");
    seqCombo->addItem("Infinite");
    seqCombo->addItem("Circular Loop");

    //Placement in layout
    layout->addWidget(lbl,0,0,1,4);
    layout->addWidget(seqLbl,2,0,1,1);
    layout->addWidget(seqCombo,3,0,3,1);
    layout->addWidget(posMouseLabel,2,3,1,1);


    //Link signals to slots
    connect(seqCombo, SIGNAL(currentTextChanged(QString)), this, SLOT(comboboxItemChanged(QString)));
}

void MemsTab::comboboxItemChanged(QString sequence)
{
    qDebug() << sequence;
}
