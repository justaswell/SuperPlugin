#include "superplugin_ui.h"


void SuperUI::click_yes()
{
    QString str;
    str=preprocess->currentText();
    preproc=new Preprocess(this->mcallback,this->datapath);
    if(str=="gaussfilter"){
        preproc->gaussfilter(datamem);
    }
    QString count;
    count=QString::fromStdString(std::to_string(datamem->getimg_cnt()));
    v3d_msg(count);
}
