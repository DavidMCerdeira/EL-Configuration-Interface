#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <QWidget>
#include <QStandardItemModel>

namespace Ui {
class EditView;
}

class EditView : public QWidget
{
    Q_OBJECT

public:
    explicit EditView(QWidget *parent = 0);
    void readXml(const QString &fileName);
    void writeXml(QString fileName);
    ~EditView();

private slots:
    void on_pushButton_clicked();

private:
    Ui::EditView *ui;

    QStandardItemModel *model;
    QString directoryRoot;

    QString fileDirectory;
};

#endif // EDITVIEW_H
