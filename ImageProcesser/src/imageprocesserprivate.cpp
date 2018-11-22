#include "imageprocesserprivate.h"
#include <QFileInfo>
#include <QThreadPool>
#include <QImage>
#include <QDebug>
#include <QDir>

typedef void (*AlgorithmFunction)(QString source, QString dest);

QEvent::Type ExcutedEvent::m_evType = QEvent::None;

/**
 * @brief _gray
 * @note 灰度处理 qGray 采用加权平均值计算灰度
 * @param source
 * @param dest
 */
static inline void _gray(QString source, QString dest)
{
    QImage image(source);

    if (image.isNull())
    {
        qDebug() << "load " << source << " failed";
        return;
    }

    for (int i = 0;i < image.width(); ++i)
    {
        for (int j = 0; j < image.height(); ++j)
        {
            QRgb pixel_color = image.pixel(i,j);
            int gray = qGray(pixel_color);
            image.setPixel(i, j, qRgba(gray, gray, gray, qAlpha(pixel_color)));
        }
    }

    image.save(dest);
}

/**
 * @brief _binarize
 * @note 黑白处理
 * @param source
 * @param dest
 */
static inline void _binarize(QString source, QString dest)
{
    QImage image(source);
    if (image.isNull())
    {
        qDebug() << "load " << source << "failed";
        return;
    }

    int height = image.height();
    int width = image.width();
    QRgb black = qRgb(0, 0, 0);
    QRgb white = qRgb(255, 255, 255);

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            QRgb pixel = image.pixel(i,j);
            QRgb avg = (qRed(pixel) + qGreen(pixel) + qBlue(pixel))/3;

            image.setPixel(i, j, (avg > 128 ? white : black));
        }
    }

    image.save(dest);
}

/**
 * @brief _negative
 * @note 底片式处理
 * @note 每个像素取255-原像素的RGB颜色
 * @param source
 * @param dest
 */
static inline void _negative(QString source, QString dest)
{
    QImage image(source);
    if (image.isNull())
    {
        qDebug() << "load " << source << "failed";
        return;
    }

    int height = image.height();
    int width = image.width();

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            QRgb pixel = image.pixel(i, j);
            QRgb negative = qRgba(255 - qRed(pixel),
                                  255 - qGreen(pixel),
                                  255 - qBlue(pixel),
                                  qAlpha(pixel));
            image.setPixel(i, j, negative);
        }
    }

    image.save(dest);
}

/**
 * @brief _emboss
 * @note 浮雕处理
 * @param source
 * @param dest
 */
static inline void _emboss(QString source, QString dest)
{
    QImage image(source);
    if (image.isNull())
    {
        qDebug() << "load " << source << "failed";
        return;
    }

    int height = image.height();
    int width = image.width();
    QRgb preColor = 0;

    for (int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            QRgb color = image.pixel(i, j);
            int r = qRed(color) - qRed(preColor) + 128;
            int g = qGreen(color) - qGreen(preColor) + 128;
            int b = qBlue(color) - qBlue(preColor) + 128;
            QRgb gray = qGray(r, g, b);
            QRgb newColor = qRgba(gray, gray, gray, qAlpha(color));
            image.setPixel(i, j, newColor);

            preColor = color;
        }
    }

    image.save(dest);
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
    if (event->type() == ExcutedEvent::evType())
    {
        ExcutedEvent *e = (ExcutedEvent *)event;

        if (m_runables.contains(e->m_runable))
        {
            Q_Q(ImageProcesser);
            m_notifySourcePath = e->m_runable->m_sourceFilePath;
            m_notifyAlgorithum = e->m_runable->m_algorithm;

            QFile file(e->m_runable->m_destFilePath);
            if(file.exists())
                emit q->finished(e->m_runable->m_destFilePath);
            else
                emit q->finished("");
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
