#ifndef MEMSTAB_H
#define MEMSTAB_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QPixmap>
#include "medialabel.h"

class MediaLabel; //foward declaration to avoid circular dependencies with medialabel.h

class MemsTab : public QWidget
{
    Q_OBJECT
public:
    explicit MemsTab(QWidget *parent = nullptr);
    MediaLabel *lbl;
    QPixmap *pix;
    QLabel *posMouseLabel;

signals:


public slots:
    void comboboxItemChanged(QString sequence);
};

#endif // MEMSTAB_H
