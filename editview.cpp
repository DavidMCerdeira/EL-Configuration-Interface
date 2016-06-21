#include "editview.h"
#include "ui_editview.h"
#include <QFile>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDomDocument>
#include <iostream>
#include <QComboBox>
#include <QSpinBox>
#include <QDebug>
#include <QTextEdit>

EditView::EditView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditView)
{
    ui->setupUi(this);

    model = new QStandardItemModel;
    ui->label->QWidget::adjustSize();
    ui->label_2->QWidget::adjustSize();
    ui->label_3->QWidget::adjustSize();
    ui->lineValueElabEdit->QWidget::adjustSize();
    ui->label_4->setAlignment(Qt::AlignCenter);

    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(1);

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Type"));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Default"));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Value"));

    this->adjustSize();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    resize(700, 500);
    setWindowIcon(QIcon(":/icon.ico"));
}

void EditView::writeXml(QString fileName)
{
    QDomDocument doc;
    int row=0;

    QDomElement root = doc.createElement("component");
    root.setAttribute("type",ui->labelCompName->text());
    doc.appendChild(root);

    QDomElement el = doc.createElement("elaboration");
    el.setAttribute("default",ui->labelDefElab->text());
    el.appendChild(doc.createTextNode(ui->lineValueElabEdit->text()));
    root.appendChild(el);
    el = doc.createElement("properties");
    root.appendChild(el);

    for(row=0; row < ui->tableWidget->rowCount(); row++)
    {
        QDomElement subel = doc.createElement("property");
        subel.setAttribute("type",ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,0)).toString());
        subel.setAttribute("name",ui->tableWidget->model()->headerData(row,Qt::Vertical).toString());
        subel.setAttribute("default",ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,1)).toString());
        el.appendChild(subel);

        QString className = ui->tableWidget->cellWidget(row,2)->metaObject()->className();

        if(className.compare("QSpinBox")==0)
        {
            QSpinBox *spin = (QSpinBox *) ui->tableWidget->cellWidget(row,2);
            QDomElement subsubel = doc.createElement("restriction");
            subsubel.setAttribute("type","range");
            subel.appendChild(subsubel);

            QDomElement subsubsubel = doc.createElement("botValue");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->minimum())));
            subsubel.appendChild(subsubsubel);
            subsubsubel = doc.createElement("topValue");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->maximum())));
            subsubel.appendChild(subsubsubel);

            subsubel = doc.createElement("value");
            subsubsubel = doc.createElement("element");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->value())));
            subsubel.appendChild(subsubsubel);
            subel.appendChild(subsubel);
        }
        else if(className.compare("QDoubleSpinBox")==0)
        {
            QDoubleSpinBox *spin = (QDoubleSpinBox *) ui->tableWidget->cellWidget(row,2);
            QDomElement subsubel = doc.createElement("restriction");
            subsubel.setAttribute("type","range");
            subel.appendChild(subsubel);

            QDomElement subsubsubel = doc.createElement("botValue");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->minimum())));
            subsubel.appendChild(subsubsubel);
            subsubsubel = doc.createElement("topValue");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->maximum())));
            subsubel.appendChild(subsubsubel);

            subsubel = doc.createElement("value");
            subsubsubel = doc.createElement("element");
            subsubsubel.appendChild(doc.createTextNode(QString::number(spin->value())));
            subsubel.appendChild(subsubsubel);
            subel.appendChild(subsubel);
        }
        else if(className.compare("QComboBox")==0)
        {
            if(ui->tableWidget->model()->data(ui->tableWidget->model()->index(row,0)).toString().compare("bool")==0)
            {
                QComboBox *combo = (QComboBox *) ui->tableWidget->cellWidget(row,2);
                QDomElement subsubel;
                subsubel = doc.createElement("value");
                QDomElement subsubsubel = doc.createElement("element");
                subsubsubel.appendChild(doc.createTextNode(combo->itemText(combo->currentIndex())));
                subsubel.appendChild(subsubsubel);
                subel.appendChild(subsubel);
            }
            else
            {
                QComboBox *combo = (QComboBox *) ui->tableWidget->cellWidget(row,2);

                QDomElement subsubel;
                for(int i=0; i< combo->count(); i++)
                {
                    subsubel = doc.createElement("element");
                    subsubel.appendChild(doc.createTextNode(combo->itemText(i)));
                    subel.appendChild(subsubel);
                }
                subsubel = doc.createElement("value");
                QDomElement subsubsubel = doc.createElement("element");
                subsubsubel.appendChild(doc.createTextNode(combo->itemText(combo->currentIndex())));
                subsubel.appendChild(subsubsubel);
                subel.appendChild(subsubel);
            }
        }
        else if(className.compare("QTextEdit")==0)
        {

            QTextEdit *text = (QTextEdit *) ui->tableWidget->cellWidget(row,2);
            qDebug() << text->toPlainText();
            QDomElement subsubel;
            subsubel = doc.createElement("value");
            QDomElement subsubsubel = doc.createElement("element");
            subsubsubel.appendChild(doc.createTextNode(text->toPlainText()));
            subsubel.appendChild(subsubsubel);
            subel.appendChild(subsubel);
        }
    }

    qDebug() << fileName;
    QFile file(fileName);

    if(file.exists())
            file.remove();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
              return;

    file.write(doc.toByteArray());
    file.close();
}

void EditView::readXml(const QString &fileName)
{
    QDomDocument doc;
    int row = 0;

    fileDirectory = fileName;

    QFile file(fileName); //filename
    if (!file.open(QIODevice::ReadOnly | QFile::Text)){
        std::cerr<< "ERROR: file openning"<< std::endl;
        return;
    }

    if (!doc.setContent(&file)) {
        std::cerr << "ERROR: dom document open" << std::endl;
        file.close();
        return;
    }

    file.close();

    QDomElement docEl = doc.documentElement();

    //Component Name
    qDebug(docEl.tagName().toLatin1() + ":" + docEl.attributeNode("type").value().toLatin1());
    ui->labelCompName->setText(docEl.attributeNode("type").value());
    setWindowTitle(tr("Edit "+docEl.attributeNode("type").value().toLatin1()+ " Component XML file"));
    ui->labelCompName->QWidget::adjustSize();


    QStringList list_rest, list_el;
    QDomNode child = docEl.firstChild();

    int i=0;
    int min=0;
    bool flag_avoid_value=false;

    while (!child.isNull())
    {
        //Elaboration element : Default : Value
        qDebug(" "+child.toElement().tagName().toLatin1() + ":" + child.toElement().attributeNode("default").value().toLatin1() + ":" + child.toElement().firstChild().nodeValue().toLatin1());
        ui->labelDefElab->setText(child.toElement().attributeNode("default").value());
        ui->labelDefElab->QWidget::adjustSize();
        ui->lineValueElabEdit->setText(child.toElement().firstChild().nodeValue());

        child = child.nextSibling();

        QDomNode childNode = child.toElement().firstChild();

        while(!childNode.isNull())
        {
            //childs ID , name, doc2, doc3 and node value respectively
            if(childNode.toElement().nodeName() == "property"){
                qDebug("  "+childNode.toElement().attributeNode("name").value().toLatin1());

                ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(childNode.toElement().attributeNode("name").value()));
                ui->tableWidget->setItem(row,0,new QTableWidgetItem(childNode.toElement().attributeNode("type").value()));
                ui->tableWidget->setItem(row,1,new QTableWidgetItem(childNode.toElement().attributeNode("default").value()));

                QDomNode subchildNode = childNode.toElement().firstChild();

                while(!subchildNode.isNull())
                {
                    QDomNode subsubchildNode = subchildNode.toElement().firstChild();

                    if(subchildNode.toElement().nodeName() == "restriction")
                    {
                        qDebug() << "OLA1";
                        qDebug("   "+subsubchildNode.toElement().nodeName().toLatin1());

                        while(!subsubchildNode.isNull())
                        {
                            qDebug("    "+subsubchildNode.toElement().firstChild().nodeValue().toLatin1());
                            if(childNode.toElement().attributeNode("type").value().compare("int")==0)
                            {
                                QDoubleSpinBox *spinItem = new QDoubleSpinBox();

                                if(i==0){
                                    min=subsubchildNode.toElement().firstChild().nodeValue().toInt();
                                    i++;
                                }
                                else{
                                    spinItem->setMaximum(subsubchildNode.toElement().firstChild().nodeValue().toInt());
                                    spinItem->setMinimum(min);
                                    i=0;
                                }

                                ui->tableWidget->setCellWidget(row,2,spinItem);
                            }
                            else if(childNode.toElement().attributeNode("type").value().compare("float")==0)
                            {
                                QSpinBox *spinItem = new QSpinBox();

                                if(i==0){
                                    min=subsubchildNode.toElement().firstChild().nodeValue().toInt();
                                    i++;
                                }
                                else{
                                    spinItem->setMaximum(subsubchildNode.toElement().firstChild().nodeValue().toInt());
                                    spinItem->setMinimum(min);
                                    i=0;
                                }

                                ui->tableWidget->setCellWidget(row,2,spinItem);
                            }
                            subsubchildNode = subsubchildNode.nextSibling();
                        }
                        flag_avoid_value=true;
                    }
                    else if(subchildNode.toElement().nodeName() == "element")
                    {
                        qDebug() << "OLA2";
                        QComboBox *combo = new QComboBox;
                        qDebug("     "+subchildNode.toElement().firstChild().nodeValue().toLatin1());
                        list_el << subchildNode.toElement().firstChild().nodeValue();
                        combo->addItems(list_el);
                        ui->tableWidget->setCellWidget(row,2,combo);
                        flag_avoid_value=true;
                    }
                    else if(!flag_avoid_value)
                    {
                        if(childNode.toElement().attributeNode("type").value().compare("int")==0)
                        {
                            QDoubleSpinBox *spinItem = new QDoubleSpinBox();

                            if(i==0){
                                min=subsubchildNode.toElement().firstChild().nodeValue().toInt();
                                i++;
                            }
                            else{
                                spinItem->setMaximum(subsubchildNode.toElement().firstChild().nodeValue().toInt());
                                spinItem->setMinimum(min);
                                i=0;
                            }

                            ui->tableWidget->setCellWidget(row,2,spinItem);
                        }
                        else if(childNode.toElement().attributeNode("type").value().compare("float")==0)
                        {
                            QSpinBox *spinItem = new QSpinBox();

                            if(i==0){
                                min=subsubchildNode.toElement().firstChild().nodeValue().toInt();
                                i++;
                            }
                            else{
                                spinItem->setMaximum(subsubchildNode.toElement().firstChild().nodeValue().toInt());
                                spinItem->setMinimum(min);
                                i=0;
                            }

                            ui->tableWidget->setCellWidget(row,2,spinItem);
                        }
                        else if(childNode.toElement().attributeNode("type").value().compare("bool")==0)
                        {
                            QComboBox *combo = new QComboBox;
                            qDebug("     "+subchildNode.toElement().firstChild().nodeValue().toLatin1());
                            combo->addItem("true");
                            combo->addItem("false");
                            ui->tableWidget->setCellWidget(row,2,combo);
                        }
                        else
                        {
                            QTextEdit *text = new QTextEdit;
                            ui->tableWidget->setCellWidget(row,2,text);
                        }
                    }
                    else
                        flag_avoid_value=false;

                    subchildNode = subchildNode.nextSibling();
                }
                row++;
                ui->tableWidget->setRowCount(row+1);
                list_el.clear();
                list_rest.clear();
            }
            childNode = childNode.nextSibling();
        }
        child = child.nextSibling();
    }

    ui->tableWidget->setRowCount(ui->tableWidget->rowCount()-1);

}

EditView::~EditView()
{
    delete ui;
}

void EditView::on_pushButton_clicked()
{
    writeXml(fileDirectory);
}
