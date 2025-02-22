#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DewaterIQ.h"

class DewaterIQ : public QMainWindow
{
    Q_OBJECT

public:
    DewaterIQ(QWidget *parent = nullptr);
    ~DewaterIQ();

private:
    Ui::DewaterIQClass ui;
};
