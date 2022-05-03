#include "superplugin_ui.h"


void SuperUI::processcmd(const V3DPluginArgList &input, V3DPluginArgList &output)
{
    inputfile=((vector<char*> *)(input.at(0).p))->at(0);                    //input.at(0)
    qinputfile=QString(inputfile);
    inputfilelist=getFileNames(qinputfile);
    for(int i=0;i<inputfilelist.size();i++){
        Image4DSimple *nimg;

        QByteArray ba1=(qinputfile+"\\"+inputfilelist[i]).toLatin1(); //+"\\"+inputfilelist[i]
        char *inputimg=ba1.data();

        char inimage[200];
        for(int i=0;i<200;i++){
            inimage[i]=inputimg[i];
            if(inputimg[i]=='\0')
                break;
        }

        nimg=mcallback->loadImage(inimage);

        datamem->push_img(nimg);

    }
    qDebug()<<datamem->getimg_cnt();


    outputfile=((vector<char*> *)(output.at(0).p))->at(0);                  //output.at(0)


    vector<char *> paras=(*(vector<char*> *)(input.at(1).p));               //input.at(1)
    vector<char *> funcparas;
    for(int i=0;i<paras.size();i++){
//        qDebug()<<paras[i][0];
        if(paras[i][0]>='a'&&paras[i][0]<='z'){         //DataFlowArg[0] is empty.
            DataFlowArg.push_back(funcparas);
            funcparas.clear();
            funcparas.push_back(paras[i]);
            continue;
        }
        funcparas.push_back(paras[i]);
        if(i==paras.size()-1)
            DataFlowArg.push_back(funcparas);
    }
//    for(int i=0;i<DataFlowArg.size();i++){
//        for (int j=0;j<DataFlowArg[i].size();j++)
//            qDebug()<<DataFlowArg[i][j];
//    }
    qDebug()<<DataFlowArg[1].size();
}

void SuperUI::initmap()
{
    fnametodll["gf"]="gaussianfilter1.dll";

    dlltomode["gaussianfilter1.dll"]="Preprocess";

}

void SuperUI::assemblyline()
{
    for(int i=1;i<DataFlowArg.size();i++){
//        V3DPluginArgList pluginInputList, pluginOutputList;
//        V3DPluginArgItem FileInput, inputParam, Output,Outputimg;
//        vector<char*> pluginInputFile;
//        vector<char*> pluginInputArgList;
//        vector<char*> pluginOutputArgList;

        vector<char*> pluginInputArgList;

        QString funcdll=fnametodll[QString(DataFlowArg[i][0])];
        QString process=dlltomode[funcdll];
        qDebug()<<funcdll;
        qDebug()<<process;
        if(process=="Preprocess"){
            if(funcdll=="gaussianfilter1.dll"){
                Preprocess * Preproc= new Preprocess(this->mcallback);
                for(int j=1;j<DataFlowArg[i].size();j++){
                    pluginInputArgList.push_back(DataFlowArg[i][j]);
                }
                for(int j=0;j<datamem->getimg_cnt();j++){
                    Preproc->gaussfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
                }
            }
        }
        for(int j=0;j<datamem->getimg_cnt();j++){
            saveresult(datamem,j);
        }

    }
}

QString SuperUI::finddll(char *funcname)
{
    return fnametodll[funcname];
}

void SuperUI::saveresult(DataFlow *data, int i)
{
    QString path=QString(outputfile)+"\\"+inputfilelist[i];
    QByteArray ba1=(path).toLatin1(); //+"\\"+inputfilelist[i]
    char *otimg=ba1.data();
    //qDebug()<<inputimg;
    char outpath[200];
    for(int i=0;i<200;i++){
        outpath[i]=otimg[i];
        if(otimg[i]=='\0')
            break;
    }
    mcallback->saveImage(data->outputimg[i],outpath);
}

void SuperUI::click_yes()
{
    QString str;
    str=preprocess->currentText();
    preproc=new Preprocess(this->mcallback);
    if(str=="gaussfilter"){
        //preproc->gaussfilter(datamem);
    }
    QString count;
    count=QString::fromStdString(std::to_string(datamem->getimg_cnt()));
    v3d_msg(count);
}
