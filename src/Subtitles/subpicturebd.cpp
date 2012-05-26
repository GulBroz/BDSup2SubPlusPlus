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

#include "subpicturebd.h"
#include "imageobject.h"

SubPictureBD::SubPictureBD()
{
}

SubPictureBD::SubPictureBD(SubPictureBD *other) :
    objectID(other->objectID),
    imageObjectList(other->imageObjectList),
    winWidth(other->winWidth),
    winHeight(other->winHeight),
    xWinOfs(other->xWinOfs),
    yWinOfs(other->yWinOfs),
    type(other->type),
    palettes(other->palettes)
{
}

int SubPictureBD::getImageWidth()
{
    return imageObjectList.at(objectID)->width;
}

int SubPictureBD::getImageHeight()
{
    return imageObjectList.at(objectID)->height;
}

int SubPictureBD::getOfsX()
{
    return imageObjectList.at(objectID)->xOfs;
}

int SubPictureBD::getOfsY()
{
    return imageObjectList.at(objectID)->yOfs;
}
