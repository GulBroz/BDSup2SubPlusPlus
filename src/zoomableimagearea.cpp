/*
 * BDSup2Sub++ (C) 2012 Adam T.
 * Based on code from BDSup2Sub by Copyright 2009 Volker Oth (0xdeadbeef)
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

#include "zoomableimagearea.h"
#include <QSize>
#include <QMessageBox>
#include <QPainter>
#include <QLinearGradient>
#include <QScrollArea>

ZoomableImageArea::ZoomableImageArea(QWidget *parent) :
    QLabel(parent)
{
}

void ZoomableImageArea::setImage(QImage* image)
{
    this->image = image;
    if (zoomScale == 0)
    {
        setZoomScale(1);
    }
    else
    {
        setZoomScale(zoomScale);
    }
}

void ZoomableImageArea::setZoomScale(int scale)
{
    if (scale != zoomScale)
    {
        zoomScale = scale;
    }
    if (image != 0 && !image->isNull())
    {
        QRect target = image->rect();
        if (zoomScale == 1)
        {
            drawPixmap = new QPixmap(this->width(), this->height());
            drawPixmap->fill();
        }
        else
        {
            target.setWidth(image->width() * zoomScale);
            target.setHeight(image->height() * zoomScale);

            int drawHeight = target.height() > originalSize.height() ? target.height() : originalSize.height();
            int drawWidth = target.width() > originalSize.width() ? target.width() : originalSize.width();
            drawPixmap = new QPixmap(drawWidth, drawHeight);
            drawPixmap->fill();
        }
        painter->begin(drawPixmap);
        QLinearGradient gradient(0, 0, drawPixmap->width(), drawPixmap->height());
        gradient.setColorAt(0, Qt::blue);
        gradient.setColorAt(1, Qt::black);
        painter->fillRect(0, 0, drawPixmap->width(), drawPixmap->height(), gradient);
        painter->drawImage(target, *image, image->rect());
        painter->end();
        this->setPixmap(*drawPixmap);
    }
}

void ZoomableImageArea::paintEvent(QPaintEvent *event)
{
    if (image == 0)
    {
        painter->begin(this);
        QLinearGradient gradient(0, 0, this->width(), this->height());
        gradient.setColorAt(0, Qt::blue);
        gradient.setColorAt(1, Qt::black);
        painter->fillRect(0, 0, this->width(), this->height(), gradient);
        painter->end();
        originalSize = this->size();
    }
    else
    {
        painter->begin(this);
        painter->drawPixmap(0, 0, *drawPixmap);
        painter->end();
    }
}

void ZoomableImageArea::mousePressEvent(QMouseEvent *event)
{
    int scale = zoomScale;
    if (event->button() == Qt::LeftButton)
    {
        if (scale < 8)
        {
            ++scale;
        }
    }
    else
    {
        if (scale > 1)
        {
            --scale;
        }
    }
    if (scale != zoomScale)
    {
        setZoomScale(scale);
    }
}
