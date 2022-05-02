#ifndef PREPROCESS_H
#define PREPROCESS_H
#include "data_io.h"
#include "v3d_interface.h"
#include "v3d_plugin_loader.h"
#include "mainwindow.h"
#include "dataflow.h"
#include <QString>


class Preprocess{
public:
    Preprocess(V3DPluginCallback2 *callback,QString root){
        mcallback=callback;
        data_paths=getFileNames(root);
        Proot=transformproot(root);
        QString count;
        count=QString::fromStdString(std::to_string(data_paths.size()));
        v3d_msg(count);
        //Predatamem=new DataFlow();
    }
    QString transformproot(QString root);
    void gaussfilter(DataFlow *Predatamem);
    void processcmd(V3DPluginArgList &input, V3DPluginArgList &output);

    QStringList data_paths;
    QString Proot;

    //DataFlow *Predatamem;
private:
    V3DPluginCallback2 *mcallback;

};

#endif // PREPROCESS_H
