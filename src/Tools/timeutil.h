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

#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <QString>
#include <QVector>
#include <QRegExp>

static QRegExp timePattern = QRegExp("(\\d+):(\\d+):(\\d+)[:\\.](\\d+)");

class TimeUtil
{
public:
    TimeUtil();

    static long timeStrToPTS(QString s);
    static QString ptsToTimeStr(long pts);
    static QVector<int> msToTime(long ms);
    static long timeStrXmlToPTS(QString s, double fps);
};

#endif // TIMEUTIL_H
