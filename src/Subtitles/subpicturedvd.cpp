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

#include "subpicturedvd.h"

SubPictureDVD::SubPictureDVD()
{
}

void SubPictureDVD::setOriginal()
{
    originalWidth = getImageWidth();
    originalHeight = getImageHeight();
    originalX = getOfsX();
    originalY = getOfsY();

    originalAlpha = alpha;
    originalPal = pal;
}

void SubPictureDVD::copyInfo(SubPicture *subPicture)
{
    width = subPicture->width;
    height = subPicture->height;
    startTime = subPicture->startTime;
    endTime = subPicture->endTime;
    isForced = subPicture->isForced;
    compNum = subPicture->compNum;
    setImageWidth(subPicture->getImageWidth());
    setImageHeight(subPicture->getImageHeight());
    setOfsX(subPicture->getOfsX());
    setOfsY(subPicture->getOfsY());
}
