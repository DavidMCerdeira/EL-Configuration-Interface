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
    typedef enum {GENERAL,SPECIFIC} READTYPE;

    explicit EditView(QWidget *parent = 0);
    void readXml(const QString &fileName, const QString &SpecificXMLFile,const QString & SpecificElabPath,READTYPE);
    void writeXml(QString fileName,READTYPE);
    ~EditView();



private slots:
    void on_pushButton_clicked();

    void on_lineValueElabEdit_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::EditView *ui;

    QList<int> list_not_restrict;

    QStandardItemModel *model;
    QString componentType;
    QString fileDirectory;
    QString SpecificElabPath;
    QString SpecificXMLFile;
    READTYPE state;

};

#endif // EDITVIEW_H
