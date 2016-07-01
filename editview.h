#ifndef EDITVIEW_H
#define EDITVIEW_H

#include <QWidget>
#include <QStandardItemModel>
#include <QStringList>

namespace Ui {
class EditView;
}

class EditView : public QWidget
{
    Q_OBJECT

public:
    typedef enum {GENERAL,SPECIFIC} READTYPE;

    explicit EditView(const QString &fileName, QString &SpecificElabPath, QWidget *parent = 0);
    void readXml(const QString &fileName, const QString &SpecificXMLFile, READTYPE RT);
    void writeXml(QString fileName,READTYPE);
    ~EditView();



private slots:
    void on_pushButton_clicked();

    void on_elabComboBox_currentIndexChanged(const QString &arg1);

    void on_openSpecificButton_clicked();

private:
    Ui::EditView *ui;

    QList<int> list_not_restrict;

    QStandardItemModel *model;
    QString componentType;
    QString fileDirectory;
    QString SpecificElabPath;
    QString SpecificXMLFile;
    READTYPE state;

    QStringList searchElaborations();

    void refreshSpecificButton(QString);
};

#endif // EDITVIEW_H
