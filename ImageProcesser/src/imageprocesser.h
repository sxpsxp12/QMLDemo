#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

#include <QObject>

class ImageProcesserPrivate;

class ImageProcesser : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString source READ sourceFile)
public:
    explicit ImageProcesser(QObject *parent = nullptr);
    ~ImageProcesser();

    enum ImageAlgorithm{
        GRAY = 0,   //灰度处理
        BINARIZE,   //黑白处理
        NEGATIVE,   //底片处理
        EMBOSS,     //浮雕处理
        SHARPEN,    //锐化处理
        SOFTEN,     //模糊处理
        ALGORITHMCOUNT = 6
    };
    Q_ENUM(ImageAlgorithm)

    QString sourceFile();

Q_SIGNALS:
    void finished(QString newFile);

    void progress(int value);

public Q_SLOTS:

    void process(QString file, ImageAlgorithm algorithm);

    void abort(QString file, ImageAlgorithm algorithm);
private:
    ImageProcesserPrivate *d_ptr;

    QString m_temp_path;
    Q_DECLARE_PRIVATE(ImageProcesser)
};

#endif // IMAGEPROCESSER_H
