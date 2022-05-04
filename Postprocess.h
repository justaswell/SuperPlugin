#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "v3d_interface.h"
#include <QString>
#include <vector>
using std::vector;

class Postprocess{
public:
    Postprocess(V3DPluginCallback2 *callback){
        mcallback=callback;
    }

private:
    V3DPluginCallback2 *mcallback;

};

#endif // POSTPROCESS_H
