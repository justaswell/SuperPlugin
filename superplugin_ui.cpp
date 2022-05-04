#include "superplugin_ui.h"


void SuperUI::processcmd(const V3DPluginArgList &input, V3DPluginArgList &output)
{
    inputfile=((vector<char*> *)(input.at(0).p))->at(0);                    //input.at(0)
    qinputfile=QString(inputfile);
    inputimglist=getImgNames(qinputfile);
    inputswclist=getSwcNames(qinputfile);
    int count=inputimglist.size()>inputswclist.size()?inputimglist.size():inputswclist.size();
    for(int i=0;i<count;i++){
        Image4DSimple *nimg=new Image4DSimple();
        NeuronTree *nswc=new NeuronTree();
        datamem->push_img(nimg);
        datamem->push_swc(nswc);
    }

    for(int i=0;i<inputimglist.size();i++){
        Image4DSimple *nimg;

        QByteArray ba1=(qinputfile+"\\"+inputimglist[i]).toLatin1(); //+"\\"+inputimglist[i]
        char *inputimg=ba1.data();

        char inimage[200];
        for(int i=0;i<200;i++){
            inimage[i]=inputimg[i];
            if(inputimg[i]=='\0')
                break;
        }

        nimg=mcallback->loadImage(inimage);

        datamem->outputimg[i]=nimg;

    }
    for(int i=0;i<inputswclist.size();i++){
        NeuronTree *nswc=new NeuronTree();
        QString swcfile=qinputfile+"\\"+inputimglist[i];

        *nswc=readSWC_file(swcfile);

        datamem->outputswc[i]=nswc;

    }



    outputfile=((vector<char*> *)(output.at(0).p))->at(0);                  //output.at(0)


    vector<char *> paras=(*(vector<char*> *)(input.at(1).p));               //input.at(1)
    vector<char *> funcparas;
    outresult=QString(paras[0]);
    qDebug()<<"Your output will be "+outresult+" format.";
    if(outresult!="img"&&outresult!="swc"){
        qDebug()<<"Wrong output format! Please input 'img' or 'swc'.";
        return;
    }

    for(int i=1;i<paras.size();i++){
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
    fnametodll["app2"]="vn21.dll";

    dlltomode["gaussianfilter1.dll"]="Preprocess";
    dlltomode["vn21.dll"]="Computation";

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
                    qDebug()<<"Executing NO. "<<i<<" func.";
                    Preproc->gaussfilter(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
                }
            }
        }else if(process=="Computation"){
            if(funcdll=="vn21.dll"){
                Computation *Comproc=new Computation(this->mcallback);
                for(int j=1;j<DataFlowArg[i].size();j++){
                    pluginInputArgList.push_back(DataFlowArg[i][j]);
                }
                for(int j=0;j<datamem->getimg_cnt();j++){
                    qDebug()<<"Executing NO. "<<i<<" func.";
                    Comproc->vn2(datamem,pluginInputArgList,j,DataFlowArg[i][0]);
                }
            }
        }else if(process=="Postprocess"){

        }

    }
    for(int j=0;j<datamem->getimg_cnt();j++){
        if(outresult=="img")
            saveimgresult(datamem,j);
        else if(outresult=="swc");
            saveswcresult(datamem,j);
    }
}

QString SuperUI::finddll(char *funcname)
{
    return fnametodll[funcname];
}

void SuperUI::saveimgresult(DataFlow *data, int i)
{
    QStringList inputlists;
    inputlists=inputimglist.size()>inputswclist.size()?inputimglist:inputswclist;
    QString path=QString(outputfile)+"\\"+inputlists[i]+"_result.tiff";
    QByteArray ba1=(path).toLatin1(); //+"\\"+inputimglist[i]
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

void SuperUI::saveswcresult(DataFlow *data, int i)
{
    QStringList inputlists;
    inputlists=inputimglist.size()>inputswclist.size()?inputimglist:inputswclist;
    QString path=QString(outputfile)+"\\"+inputlists[i]+"_result.swc";
    QStringList infostring;
    infostring.push_back("##Assembly line");
    infostring.push_back("##Output by superplugin");
    qDebug()<<writeSWC_file(path,*data->outputswc[i]);

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
