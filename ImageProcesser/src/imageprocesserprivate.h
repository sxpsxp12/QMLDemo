#ifndef IMAGEPROCESSERPRIVATE_H
#define IMAGEPROCESSERPRIVATE_H

#include <QObject>
#include <QRunnable>
#include <QCoreApplication>
#include <QEvent>
#include "imageprocesser.h"
#include <QVector>
#include <QPointer>

class AlgorithmRunnable;

class ExcutedEvent: public QEvent
{
public:
    ExcutedEvent(AlgorithmRunnable *r):
        QEvent(evType()),m_runable(r)
    {

    }

    AlgorithmRunnable *m_runable;
    static QEvent::Type evType(){
        if(m_evType == QEvent::None)
            m_evType = (QEvent::Type)registerEventType();

        return m_evType;
    }

private:
    static QEvent::Type m_evType;
};

class AlgorithmRunnable:public QRunnable
{
public:
    AlgorithmRunnable(QString source, QString dest,
                      ImageProcesser::ImageAlgorithm algorithm, QObject *observer);
    ~AlgorithmRunnable(){}

    QString m_destFilePath;

    void run() override;

    QPointer<QObject> m_observer;
    QString m_sourceFilePath;
    ImageProcesser::ImageAlgorithm m_algorithm;
};

class ImageProcesserPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ImageProcesserPrivate(ImageProcesser *processer);
    ~ImageProcesserPrivate();

    void process(QString source, ImageProcesser::ImageAlgorithm algorithm);

protected:
    bool event(QEvent *event) override;
private:
    ImageProcesser *q_ptr;
    Q_DECLARE_PUBLIC(ImageProcesser)

    QVector<AlgorithmRunnable *> m_runables;
    QString m_temp_path;
    ImageProcesser::ImageAlgorithm m_notifyAlgorithum;
    QString m_notifySourcePath;
};

#endif // IMAGEPROCESSERPRIVATE_H
