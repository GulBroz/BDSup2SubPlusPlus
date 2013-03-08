/*
 * BDSup2Sub++ (C) 2012 Adam T.
 * Based on code from BDSup2Sub by Copyright 2009 Volker Oth (0xdeadbeef)
 * and Copyright 2012 Miklos Juhasz (mjuhasz)
 *
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SUBPICTURE_H
#define SUBPICTURE_H

#include "erasepatch.h"

#include <QVector>
#include <QRect>

class SubPicture
{
public:
    SubPicture();
    SubPicture(const SubPicture& other);
    SubPicture(const SubPicture* other);
    virtual ~SubPicture() { }

    int screenWidth() { return _screenWidth; }
    void setScreenWidth(int width) { _screenWidth = width; }

    int screenHeight() { return _screenHeight; }
    void setScreenHeight(int height) { _screenHeight = height; }

    virtual int imageWidth() { return _imageWidth; }
    void setImageWidth(int w) { _imageWidth = w; }

    virtual int imageHeight() {  return _imageHeight; }
    void setImageHeight(int h) { _imageHeight = h; }

    virtual int x() { return xOfs; }
    void setX(int ofs) { xOfs = ofs; }

    virtual int y() { return yOfs; }
    void setY(int ofs) { yOfs = ofs; }

    qint64 startTime() { return start; }
    void setStartTime(qint64 startTime) { start = startTime; }

    qint64 endTime() { return end; }
    void setEndTime(qint64 endTime) { end = endTime; }

    int compNum() { return compositionNumber; }
    void setCompNum(int compNum) { compositionNumber = compNum; }

    int numCompObjects() { return numberCompObjects; }
    void setNumCompObjects(int numCompObjects) { numberCompObjects = numCompObjects; }

    virtual bool isForced() { return forced; }
    virtual void setForced(bool isForced) { forced = isForced; }

    bool wasDecoded() { return decoded; }
    void setDecoded(bool wasDecoded) { decoded = wasDecoded; }

    bool exclude() { return excluded; }
    void setExclude(bool exclude) { excluded = exclude; }

    QVector<QRect> &windowSizes() { return scaledImageRects; }
    void setWindowSizes(QVector<QRect> rects) { scaledImageRects = imageRects = rects; }

    void scaleWindows(double scaleXFactor, double scaleYFactor)
    {
        if (imageRects.size() == 0) return;

        if (scaleXFactor == 1.0d && scaleYFactor)
        {
            scaledImageRects = imageRects;
        }
        else
        {
            for (int i = 0; i < imageRects.size(); ++i)
            {
                scaledImageRects[i].setX((int) ((imageRects[i].x() * scaleXFactor) + .5));
                scaledImageRects[i].setY((int) ((imageRects[i].y() * scaleYFactor) + .5));
                scaledImageRects[i].setWidth((int) ((imageRects[i].width() * scaleXFactor) + .5));
                scaledImageRects[i].setHeight((int) ((imageRects[i].height() * scaleYFactor) + .5));
            }
        }
    }

    virtual SubPicture* copy();

    QVector<ErasePatch*> erasePatch;

protected:
    int _imageWidth = 0;
    int _imageHeight = 0;
    int xOfs = 0;
    int yOfs = 0;

    QVector<QRect> scaledImageRects;
    QVector<QRect> imageRects;

private:
    int _screenWidth = 0;
    int _screenHeight = 0;
    qint64 start = -1;
    qint64 end = 0;
    int compositionNumber = 0;
    int numberCompObjects = 0;

    bool forced = false;
    bool decoded = false;
    bool excluded = false;
};

#endif // SUBPICTURE_H
