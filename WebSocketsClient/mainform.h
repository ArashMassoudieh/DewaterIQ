#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "aquatable.h"
#include "aquatablemodel.h"


class QTableView;
class AquaPlotter;

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

private:
    Ui::MainForm *ui;
    bool isValidJson(const QString &jsonString);
    AquaTableModel *tablemodel = nullptr;
    AquaTable *table = nullptr;
    QTableView *tableviewer = nullptr;
    AquaPlotter *chart = nullptr;
public slots:
    void onTextMessageRecieved(const QString &msg);
};

#endif // MAINFORM_H
