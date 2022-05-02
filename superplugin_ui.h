#ifndef SUPERPLUGIN_UI_H
#define SUPERPLUGIN_UI_H
#include <QWidget>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QString>
#include <QFileDialog>
#include <v3d_interface.h>
#include "Preprocess.h"
//#include "dataflow.h"


class SuperUI:public QWidget{
    Q_OBJECT
public:
    explicit SuperUI(V3DPluginCallback2 &callback, QWidget *parent){
        this->mcallback=&callback;
        this->mparent=parent;
        datapath=QFileDialog::getExistingDirectory(this,"data_path","d:");
        this->drawlayout();
        datamem=new DataFlow();
    }

    void drawlayout(){
        this->setFixedSize(480,360);
        this->setWindowTitle("SuperPlugin");
        preprocess=new QComboBox();
        preprocess->addItem(QWidget::tr("gaussfilter"));
        preprocess->addItem(QWidget::tr("imPreprocess"));
        hlayout=new QHBoxLayout();
        hlayout->addWidget(preprocess);
        confirm=new QPushButton(QWidget::tr("Yes"));
        vlayout=new QVBoxLayout();
        vlayout->addWidget(preprocess);
        vlayout->addWidget(confirm);
        this->setLayout(vlayout);

        connect(confirm,SIGNAL(clicked()),this,SLOT(click_yes()));
    }



public slots:
    void click_yes();

private:
    V3DPluginCallback2 *mcallback;
    QWidget *mparent;

    QHBoxLayout *hlayout;
    QVBoxLayout *vlayout;

    QString datapath;

    QComboBox *preprocess;
    QPushButton *confirm;

    DataFlow *datamem;

    Preprocess *preproc;
};

#endif // SUPERPLUGIN_UI_H
