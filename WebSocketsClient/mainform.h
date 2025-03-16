#ifndef MAINFORM_H
#define MAINFORM_H

#include <QDialog>
#include "aquatable.h"
#include "aquatablemodel.h"


class QTableView;

namespace Ui {
class MainForm;
}

class MainForm : public QDialog
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
public slots:
    void onTextMessageRecieved(const QString &msg);
};

#endif // MAINFORM_H
