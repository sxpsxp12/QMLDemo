#include "imageprocesserprivate.h"
#include <QFileInfo>
#include <QThreadPool>
#include <QImage>
#include <QDebug>
#include <QDir>

typedef void (*AlgorithmFunction)(QString source, QString dest);

QEvent::Type ExcutedEvent::m_evType = QEvent::None;

static inline void _gray(QString source, QString dest)
{
    QImage image(source);

    if(image.isNull())
    {
        qDebug() << "load " << source << " failed";
        return;
    }

    for(int i = 0;i < image.width(); ++i)
    {
        for(int j = 0; j < image.height(); ++j)
        {
            QRgb pixel_color = image.pixel(i,j);
            int gray = qGray(pixel_color);
            image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(pixel_color)));
        }
    }

    image.save(dest);
}

static inline void _binarize(QString source, QString dest)
{

}

static inline void _negative(QString source, QString dest)
{

}

static inline void _emboss(QString source, QString dest)
{

}

static inline void _sharpen(QString source, QString dest)
{

}

static inline void _soften(QString source, QString dest)
{

}

static AlgorithmFunction g_function[ImageProcesser::ALGORITHMCOUNT] = {
    _gray,
    _binarize,
    _negative,
    _emboss,
    _sharpen,
    _soften
};

ImageProcesserPrivate::ImageProcesserPrivate(ImageProcesser *processer) :
    QObject(processer),q_ptr(processer)
{

}

ImageProcesserPrivate::~ImageProcesserPrivate()
{
}

void ImageProcesserPrivate::process(QString source, ImageProcesser::ImageAlgorithm algorithm)
{
    source.remove("file:///");

    QFileInfo fi(source);
    QString tempPath = fi.dir().absolutePath();

    QString destFilePath = QString("%1/%2_%3").arg(tempPath).arg((int)algorithm)
            .arg(fi.fileName());
    AlgorithmRunnable *runable = new AlgorithmRunnable(source, destFilePath, algorithm, this);

    m_runables.append(runable);
    runable->setAutoDelete(false);

    QThreadPool::globalInstance()->start(runable);
}


bool ImageProcesserPrivate::event(QEvent *event)
{
    if(event->type() == ExcutedEvent::evType())
    {
        ExcutedEvent *e = (ExcutedEvent *)event;

        if(m_runables.contains(e->m_runable))
        {
            Q_Q(ImageProcesser);
            m_notifySourcePath = e->m_runable->m_sourceFilePath;
            m_notifyAlgorithum = e->m_runable->m_algorithm;
            emit q->finished(e->m_runable->m_destFilePath);
            m_runables.removeOne(e->m_runable);
        }

        delete e->m_runable;
        return true;
    }
    return QObject::event(event);
}

AlgorithmRunnable::AlgorithmRunnable(QString source, QString dest, ImageProcesser::ImageAlgorithm algorithm, QObject *observer):
    m_sourceFilePath(source),m_destFilePath(dest),m_algorithm(algorithm),m_observer(observer)
{

}

void AlgorithmRunnable::run()
{
    g_function[m_algorithm](m_sourceFilePath, m_destFilePath);
    QCoreApplication::postEvent(m_observer, new ExcutedEvent(this));
}
