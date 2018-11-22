#include "imageprocesser.h"
#include "imageprocesserprivate.h"

ImageProcesser::ImageProcesser(QObject *parent) :
    QObject(parent),d_ptr(new ImageProcesserPrivate(this))
{

}

ImageProcesser::~ImageProcesser()
{
    Q_D(ImageProcesser);

    d->deleteLater();
}

QString ImageProcesser::sourceFile()
{
    Q_D(ImageProcesser);

    return d->m_notifySourcePath;
}

void ImageProcesser::process(QString file, ImageProcesser::ImageAlgorithm algorithm)
{
    Q_D(ImageProcesser);

    d->process(file, algorithm);
}

void ImageProcesser::abort(QString file, ImageProcesser::ImageAlgorithm algorithm)
{
    Q_D(ImageProcesser);

    int size = d->m_runables.length();

    for (int i = 0;i < size; ++i)
    {
        if (d->m_runables.at(i)->m_sourceFilePath == file
                && d->m_runables.at(i)->m_algorithm)
        {
            d->m_runables.removeAt(i);
            break;
        }
    }
}
