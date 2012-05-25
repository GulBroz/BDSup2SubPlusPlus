#ifndef SUPXML_H
#define SUPXML_H

#include "substream.h"
#include "types.h"
#include <QObject>
#include <QFile>
#include <QtXml/QXmlDefaultHandler>
#include <QStringList>
#include <QString>

class SubtitleProcessor;
class SubPictureXML;
class QImage;
class BitStream;
class XmlHandler;

class SupXML : public QObject, public Substream
{
    Q_OBJECT

    class XmlHandler : public QXmlDefaultHandler
    {
    public:
        XmlHandler(SupXML* parent) { this->parent = parent; }
        bool characters(const QString &ch);
        bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
        bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);

    private:
        enum class XmlState { BDN, DESCRIPT, NAME, LANGUAGE, FORMAT, EVENTS, EVENT, GRAPHIC, UNKNOWN };
        QStringList xmlStates = { "bdn", "description", "name", "language", "format", "events", "event", "graphic" };

        XmlState state;
        QString txt;
        bool valid;
        SubPictureXML* subPicture;
        SupXML* parent;

        XmlState findState(QString string);
        Resolution getResolution (QString string);
        QVector<int> getResolutions(Resolution resolution);
    };

public:
    SupXML(QString fileName, SubtitleProcessor* subtitleProcessor);

    Palette *getPalette();
    Bitmap *getBitmap();
    QImage *getImage();
    QImage *getImage(Bitmap *bitmap);
    int getPrimaryColorIndex();
    void decode(int index);
    int getNumFrames();
    int getNumForcedFrames();
    bool isForced(int index);
    void close();
    long getEndTime(int index);
    long getStartTime(int index);
    long getStartOffset(int index);
    SubPicture *getSubPicture(int index);

    void readAllImages();
    QString getLanguage() { return language; }
    double getFps() { return fps; }

signals:
    void maxProgressChanged(int maxProgress);
    void currentProgressChanged(int currentProgress);

private:
    QString xmlFileName;
    QFile xmlFile;
    QString title = "";
    QString pathName;
    QString language = "deu";
    Resolution resolution = Resolution::HD_1080;
    QVector<SubPictureXML*> subPictures = QVector<SubPictureXML*>();
    FileBuffer *fileBuffer = 0;
    Palette *palette = 0;
    Bitmap *bitmap = 0;
    int primaryColorIndex = 0;
    int numToImport = 0;
    double fps = FPS_24P;
    double fpsXml;

    double XmlFps(double fps);
};

#endif // SUPXML_H
