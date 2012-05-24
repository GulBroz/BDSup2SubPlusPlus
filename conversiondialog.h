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

#ifndef CONVERSIONDIALOG_H
#define CONVERSIONDIALOG_H

#include "types.h"

#include <QDialog>

class SubtitleProcessor;

namespace Ui {
class ConversionDialog;
}

class ConversionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConversionDialog(QWidget *parent = 0, SubtitleProcessor *subtitleProcessor = 0);
    ~ConversionDialog();

    void enableOptionMove(bool enable);
    
private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

private:
    Ui::ConversionDialog *ui;
    SubtitleProcessor *subtitleProcessor;

    QColor errBackground = QColor(0xffe1acac);
    QColor warnBackground = QColor(0xffffffc0);
    QColor okBackground = QColor(0xff000000);

    Resolution resolution;
    int delayPTS;
    int minTimePTS;
    bool changeFPS;
    bool changeResolution;
    bool fixShortFrames;
    double fpsSrc;
    double fpsTrg;
    bool cancel;
    bool isReady = false;
    bool changeScale;
    double scaleX;
    double scaleY;
    bool fpsSrcCertain;
    SetState forcedState;
    bool moveCaptions;
};

#endif // CONVERSIONDIALOG_H
