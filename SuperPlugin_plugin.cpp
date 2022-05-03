/* SuperPlugin_plugin.cpp
 * A
data production line.
 * 2022-4-26 : by csz,dlc,ljs
 */
 
#include "v3d_message.h"
#include <vector>
#include "SuperPlugin_plugin.h"
using namespace std;
//Q_EXPORT_PLUGIN2(SuperPlugin, SuperPlugin);
 
QStringList SuperPlugin::menulist() const
{
	return QStringList() 
		<<tr("autoproduce")
		<<tr("preprocess")
		<<tr("computation")
		<<tr("postprocess")
		<<tr("about");
}

QStringList SuperPlugin::funclist() const
{
	return QStringList()
		<<tr("autoproduce")
		<<tr("preprocess")
		<<tr("computation")
		<<tr("postprocess")
		<<tr("help");
}

void SuperPlugin::domenu(const QString &menu_name, V3DPluginCallback2 &callback, QWidget *parent)
{
	if (menu_name == tr("autoproduce"))
	{
		v3d_msg("To be implemented.");
	}
	else if (menu_name == tr("preprocess"))
	{
        this->spui=new SuperUI(callback,parent);
        this->spui->show();
        //v3d_msg("To be implemented.");
	}
	else if (menu_name == tr("computation"))
	{
		v3d_msg("To be implemented.");
	}
	else if (menu_name == tr("postprocess"))
	{
		v3d_msg("To be implemented.");
	}
	else
	{
        v3d_msg(tr("A data production line.. "
			"Developed by csz,dlc,ljs, 2022-4-26"));
	}
}

bool SuperPlugin::dofunc(const QString & func_name, const V3DPluginArgList & input, V3DPluginArgList & output, V3DPluginCallback2 & callback,  QWidget * parent)
{
	vector<char*> infiles, inparas, outfiles;
	if(input.size() >= 1) infiles = *((vector<char*> *)input.at(0).p);
	if(input.size() >= 2) inparas = *((vector<char*> *)input.at(1).p);
	if(output.size() >= 1) outfiles = *((vector<char*> *)output.at(0).p);

	if (func_name == tr("autoproduce"))
	{
        this->spui=new SuperUI(callback,input,output);
        this->spui->assemblyline();
        //v3d_msg("To be implemented.");
	}
	else if (func_name == tr("preprocess"))
	{
		v3d_msg("To be implemented.");
	}
	else if (func_name == tr("computation"))
	{
		v3d_msg("To be implemented.");
	}
	else if (func_name == tr("postprocess"))
	{
		v3d_msg("To be implemented.");
	}
	else if (func_name == tr("help"))
	{
		v3d_msg("To be implemented.");
	}
	else return false;

	return true;
}

