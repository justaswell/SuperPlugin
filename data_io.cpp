#include "data_io.h"

QStringList getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.v3draw"<<"*.tiff";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}
