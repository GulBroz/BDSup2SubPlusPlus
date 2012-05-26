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

#include "supbd.h"

#include "subtitleprocessor.h"
#include "Tools/fileBuffer.h"
#include "Tools/timeutil.h"
#include "subpicturebd.h"
#include "imageobject.h"
#include "imageobjectfragment.h"
#include "bitmap.h"
#include "types.h"

#include <QImage>

SupBD::SupBD(QString fileName, SubtitleProcessor* subtitleProcessor) :
    supFileName(fileName)
{
    this->subtitleProcessor = subtitleProcessor;
}

Palette *SupBD::getPalette()
{
    return palette;
}

Bitmap *SupBD::getBitmap()
{
    return bitmap;
}

QImage *SupBD::getImage()
{
    return bitmap->getImage(palette);
}

QImage *SupBD::getImage(Bitmap *bitmap)
{
    return bitmap->getImage(palette);
}

int SupBD::getPrimaryColorIndex()
{
    return primaryColorIndex;
}

void SupBD::decode(int index)
{
    if (index < subPictures.size())
    {
        decode(subPictures.at(index));
    }
    else
    {
        //TODO: error handling
        throw 10;
    }
}

int SupBD::getNumFrames()
{
    return subPictures.size();
}

int SupBD::getNumForcedFrames()
{
    //TODO: implement
    throw 10;
}

bool SupBD::isForced(int index)
{
    //TODO: implement
    throw 10;
}

void SupBD::close()
{
    fileBuffer->close();
}

long SupBD::getEndTime(int index)
{
    //TODO: implement
    throw 10;
}

long SupBD::getStartTime(int index)
{
    //TODO: implement
    throw 10;
}

long SupBD::getStartOffset(int index)
{
    subPictures.at(index)->getImgObj()->fragmentList.at(0)->imageBufferOfs;
}

SubPicture *SupBD::getSubPicture(int index)
{
    SubPictureBD* subPic = subPictures.at(index);
    return subPic;
}

void SupBD::readAllSupFrames()
{
    int index = 0;
    fileBuffer = new FileBuffer(supFileName);
    int bufsize = (int)fileBuffer->getSize();
    SupBD::SupSegment* segment;
    SubPictureBD* pic = 0;
    SubPictureBD* picLast = 0;
    SubPictureBD* picTmp = 0;
    subPictures = QVector<SubPictureBD*>();
    int odsCtr = 0;
    int pdsCtr = 0;
    int odsCtrOld = 0;
    int pdsCtrOld = 0;
    int compNum = -1;
    int compNumOld = -1;
    int compCount = 0;
    long ptsPCS = 0;
    bool paletteUpdate = false;
    CompositionState cs = CompositionState::INVALID;

    while (index < bufsize)
    {
        if (subtitleProcessor->isCancelled())
        {
            //TODO: print message about user cancelled loading
            throw 10;
        }
        emit currentProgressChanged(index);
        segment = readSegment(index);
        QString out;
        QString so; // hack to return string
        switch (segment->type)
        {
        case 0x14:
        {
            out = QString("PDS ofs:%1, size:%2").arg(QString::number(index, 16), 8, QChar('0'))
                                                .arg(QString::number(segment->size, 16), 4, QChar('0'));
            if (compNum != compNumOld)
            {
                if (pic != 0)
                {
                    so = QString("");
                    int ps = parsePDS(segment, pic, so);
                    if (ps >= 0)
                    {
                        //TODO: print line
                        if (ps > 0) // don't count empty palettes
                        {
                            pdsCtr++;
                        }
                    }
                    else
                    {
                        //TODO: print lines
                    }
                }
                else
                {
                    //TODO: print lines
                }
            }
            else
            {
                //TODO: print line
            }
        } break;
        case 0x15:
        {
            out = QString("ODS ofs:%1, size%2").arg(QString::number(index, 16), 8, QChar('0'))
                                               .arg(QString::number(segment->size, 16), 4, QChar('0'));
            if (compNum != compNumOld)
            {
                if (!paletteUpdate)
                {
                    if (pic != 0)
                    {
                        so = QString("");
                        if (parseODS(segment, pic, so))
                        {
                            odsCtr++;
                        }
                        if (!so.isNull())
                        {
                            out += ", "+so;
                        }
                        //TODO: print line
                    }
                    else
                    {
                        //TODO: print lines
                    }
                }
                else
                {
                    //TODO: print lines
                }
            }
            else
            {
                //TODO: print line
            }
        } break;
        case 0x16:
        {
            compNum = fileBuffer->getWord(segment->offset + 5);
            cs = getCompositionState(segment);
            paletteUpdate = fileBuffer->getByte(segment->offset + 8) == 0x80;
            ptsPCS = segment->timestamp;
            if (segment->size >= 0x13)
            {
                compCount = 1; // could be also 2, but we'll ignore this for the moment
            }
            else
            {
                compCount = 0;
            }
            if (cs == CompositionState::INVALID)
            {
                //TODO: print line
            }
            else if (cs == CompositionState::EPOCH_START)
            {
                // new frame
                if (subPictures.size() > 0 && (odsCtr==0 || pdsCtr==0))
                {
                    //TODO: print line
                    subPictures.remove(subPictures.size()-1);
                    compNumOld = compNum-1;
                    if (subPictures.size() > 0)
                    {
                        picLast = subPictures.at(subPictures.size()-1);
                    }
                    else
                    {
                        picLast = 0;
                    }
                }
                else
                {
                    picLast = pic;
                }
                pic = new SubPictureBD();
                subPictures.push_back(pic); // add to list
                pic->startTime = segment->timestamp;
                //TODO: print line

                so = QString("");
                parsePCS(segment, pic, so);
                // fix end time stamp of previous pic if still missing
                if (picLast != 0 && picLast->endTime == 0)
                {
                    picLast->endTime = pic->startTime;
                }

                out = QString("PCS ofs:%1, START, size:%2, comp#: %3, forced: %4").arg(QString::number(index, 16), 8, QChar('0'))
                                                                                  .arg(QString::number(segment->size, 16), 4, QChar('0'))
                                                                                  .arg(QString::number(compNum))
                                                                                  .arg(pic->isForced ? "true" : "false");
                if (!so.isNull())
                {
                    out+=", "+so+"\n";
                }
                else
                {
                    out+="\n";
                }
                out += QString("PTS start: %1").arg(TimeUtil::ptsToTimeStr(pic->startTime));
                out += QString(", screen size: %1*%2\n").arg(QString::number(pic->width)).arg(QString::number(pic->height));
                odsCtr = 0;
                pdsCtr = 0;
                odsCtrOld = 0;
                pdsCtrOld = 0;
                picTmp = 0;
                //TODO: print line
            }
            else
            {
                if (pic == 0)
                {
                    //TODO: print line
                    break;
                }
                out = QString("PCS ofs:%1, ").arg(QString::number(index, 16), 8, QChar('0'));
                switch ((int)cs)
                {
                case (int)CompositionState::EPOCH_CONTINUE:
                {
                    out += QString("CONT, ");
                } break;
                case (int)CompositionState::ACQU_POINT:
                {
                    out += QString("ACQU, ");
                } break;
                case (int)CompositionState::NORMAL:
                {
                    out += QString("NORM, ");
                } break;
                }
                out += QString(" size: %1, comp#: %2, forced: %3").arg(QString::number(segment->size, 16), 4, QChar('0'))
                                                                  .arg(QString::number(compNum))
                                                                  .arg(pic->isForced ? "true" : "false");
                if (compNum != compNumOld)
                {
                    so = QString("");
                    // store state to be able to revert to it
                    picTmp = new SubPictureBD(pic);
                    picTmp->endTime = ptsPCS;
                    // create new pic
                    parsePCS(segment, pic, so);
                }
                if (!so.isNull())
                {
                    out+=", "+so;
                }
                out += QString(", pal update: %1\n").arg(paletteUpdate ? "true" : "false");
                out += QString("PTS: %1\n").arg(TimeUtil::ptsToTimeStr(segment->timestamp));
                //TODO: print line
            }
        } break;
        case 0x17:
        {
            out = QString("WDS ofs:%1, size:%2").arg(QString::number(index, 16), 8, QChar('0'))
                                                .arg(QString::number(segment->size, 16), 4, QChar('0'));
            if (pic != 0)
            {
                parseWDS(segment, pic);
                //TODO: print line
            }
            else
            {
                //TODO: print lines
            }
        } break;
        case 0x80:
        {
            //TODO: print line
            // decide whether to store this last composition section as caption or merge it
            if (cs == CompositionState::EPOCH_START)
            {
                if (compCount>0 && odsCtr>odsCtrOld
                                && compNum != compNumOld
                                && picMergable(picLast, pic))
                {
                    // the last start epoch did not contain any (new) content
                    // and should be merged to the previous frame
                    subPictures.remove(subPictures.size() - 1);
                    pic = picLast;
                    if (subPictures.size() > 0)
                    {
                        picLast = subPictures.at(subPictures.size()-1);
                    }
                    else
                    {
                        picLast = 0;
                    }
                    //TODO: print line
                }
            }
            else
            {
                long startTime = 0;
                if (pic != 0)
                {
                    startTime = pic->startTime;  // store
                    pic->startTime = ptsPCS;    // set for testing merge
                }

                if (compCount>0 && odsCtr>odsCtrOld
                                && compNum != compNumOld
                                && !picMergable(picTmp, pic))
                {
                    // last PCS should be stored as separate caption
                    if (odsCtr - odsCtrOld > 1 || pdsCtr - pdsCtrOld > 1)
                    {
                        //TODO: print line
                    }
                    // replace pic with picTmp (deepCopy created before new PCS)
                    subPictures.replace(subPictures.size() - 1, picTmp); // replace in list
                    picLast = picTmp;
                    subPictures.push_back(pic); // add to list
                    //TODO: print line
                    odsCtrOld = odsCtr;
                }
                else
                {
                    if (pic != 0)
                    {
                        // merge with previous pic
                        pic->startTime = startTime; // restore
                        pic->endTime = ptsPCS;
                        // for the unlikely case that forced flag changed during one captions
                        if (picTmp != 0 && picTmp->isForced)
                        {
                            pic->isForced = true;
                        }
                        if (pdsCtr > pdsCtrOld || paletteUpdate)
                        {
                            //TODO: print line
                        }
                    }
                    else
                    {
                        //TODO: print line
                    }
                }
            }
            pdsCtrOld = pdsCtr;
            compNumOld = compNum;
        } break;
        default:
        {
            //TODO: print warning
        } break;
        }
        index += 13; // header size
        index += segment->size;
    }

    // check if last frame is valid
    if (subPictures.size() > 0 && (odsCtr == 0 || pdsCtr == 0))
    {
        //TODO: print line
        subPictures.remove(subPictures.size()-1);
    }

    emit currentProgressChanged(bufsize);
    // count forced frames
    numForcedFrames = 0;
    for (auto subPicture : subPictures)
    {
        if (subPicture->isForced)
        {
            numForcedFrames++;
        }
    }
    //TODO: print line
}

double SupBD::getFps(int index)
{
    return getFpsFromID(subPictures.at(index)->type);
}

SupBD::SupSegment *SupBD::readSegment(int offset)
{
    SupSegment* segment = new SupSegment;
    if (fileBuffer->getWord(offset) != 0x5047)
    {
        //TODO: error handling
        throw 10;
    }
    segment->timestamp = fileBuffer->getDWord(offset += 2); // read PTS
    offset += 4; /* ignore DTS */
    segment->type = fileBuffer->getByte(offset += 4);
    segment->size = fileBuffer->getWord(++offset);
    segment->offset = offset + 2;
    return segment;
}

bool SupBD::picMergable(SubPictureBD* a, SubPictureBD* b)
{
    bool eq = false;
    if (a != 0 && b != 0)
    {
        if (a->endTime == 0 || (b->startTime - a->endTime) < subtitleProcessor->getMergePTSdiff())
        {
            ImageObject* ao = a->getImgObj();
            ImageObject* bo = b->getImgObj();
            if (ao != 0 && bo != 0)
            {
                if ((ao->bufferSize == bo->bufferSize) && (ao->width == bo->width) && (ao->height == bo->height))
                {
                    eq = true;
                }
            }
        }
    }
    return eq;
}

void SupBD::parsePCS(SupSegment* segment, SubPictureBD* subPicture, QString msg)
{
    int index = segment->offset;
    if (segment->size >= 4)
    {
        subPicture->width  = fileBuffer->getWord(index);            // video_width
        subPicture->height = fileBuffer->getWord(index + 2);        // video_height
        int type = fileBuffer->getByte(index + 4);                  // hi nibble: frame_rate, lo nibble: reserved
        int num  = fileBuffer->getWord(index + 5);                  // composition_number
        // skipped:
        // 8bit  composition_state: 0x00: normal, 		0x40: acquisition point
        //							0x80: epoch start,	0xC0: epoch continue, 6bit reserved
        // 8bit  palette_update_flag (0x80), 7bit reserved
        int palID = fileBuffer->getByte(index + 9);                 // 8bit  palette_id_ref
        int coNum = fileBuffer->getByte(index + 10);                // 8bit  number_of_composition_objects (0..2)
        if (coNum > 0)
        {
            // composition_object:
            int objID = fileBuffer->getWord(index + 11);            // 16bit object_id_ref
            msg = QString("palID: %1, objID: %2").arg(QString::number(palID)).arg(QString::number(objID));
            ImageObject* imgObj;
            if (objID >= subPicture->imageObjectList.size())
            {
                imgObj = new ImageObject;
                subPicture->imageObjectList.push_back(imgObj);
            }
            else
            {
                imgObj = subPicture->getImgObj(objID);
            }
            imgObj->paletteID = palID;
            subPicture->objectID = objID;

            // skipped:  8bit  window_id_ref
            if (segment->size >= 0x13)
            {
                subPicture->type = type;
                // object_cropped_flag: 0x80, forced_on_flag = 0x040, 6bit reserved
                int forcedCropped = fileBuffer->getByte(index + 14);
                subPicture->compNum = num;
                subPicture->isForced = ((forcedCropped & 0x40) == 0x40);
                imgObj->xOfs  = fileBuffer->getWord(index + 15);     // composition_object_horizontal_position
                imgObj->yOfs = fileBuffer->getWord(index + 17);		// composition_object_vertical_position
            }
        }
    }
}

int SupBD::parsePDS(SupSegment *segment, SubPictureBD *subPicture, QString msg)
{
    int index = segment->offset;
    int paletteID = fileBuffer->getByte(index);	// 8bit palette ID (0..7)
    // 8bit palette version number (incremented for each palette change)
    int paletteUpdate = fileBuffer->getByte(index + 1);
    if (subPicture->palettes.isEmpty())
    {
        subPicture->palettes = QVector<QVector<PaletteInfo*>*>();
        for (int i = 0; i < 8; ++i)
        {
            subPicture->palettes.push_back(new QVector<PaletteInfo*>);
        }
    }
    if (paletteID > 7)
    {
        msg = QString("Illegal palette id at offset %1").arg(QString::number(index, 16), 8, QChar('0'));
        return -1;
    }
    //
    QVector<PaletteInfo*>* al = subPicture->palettes.at(paletteID);
    if (al == 0)
    {
        al = new QVector<PaletteInfo*>;
    }
    PaletteInfo* p = new PaletteInfo;
    p->paletteSize = (segment->size - 2) / 5;
    p->paletteOfs = index + 2;
    al->push_back(p);
    msg = QString("ID: %1, update: %2, %3 entries").arg(QString::number(paletteID))
                                                   .arg(QString::number(paletteUpdate))
                                                   .arg(QString::number(p->paletteSize));
    return p->paletteSize;
}

bool SupBD::parseODS(SupSegment *segment, SubPictureBD* subPicture, QString msg)
{
    int index = segment->offset;
    ImageObjectFragment* info;
    int objID = fileBuffer->getWord(index);         // 16bit object_id
    int objVer = fileBuffer->getByte(index+1);		// 16bit object_id
    int objSeq = fileBuffer->getByte(index+3);		// 8bit  first_in_sequence (0x80),
                                                    // last_in_sequence (0x40), 6bits reserved
    bool first = ((objSeq & 0x80) == 0x80);
    bool last  = ((objSeq & 0x40) == 0x40);

    ImageObject* imgObj;
    if (objID >= subPicture->imageObjectList.size())
    {
        imgObj = new ImageObject;
        subPicture->imageObjectList.push_back(imgObj);
    }
    else
    {
        imgObj = subPicture->getImgObj(objID);
    }

    if (imgObj->fragmentList.isEmpty() || first)       // 8bit  object_version_number
    {
        // skipped:
        //  24bit object_data_length - full RLE buffer length (including 4 bytes size info)
        int width  = fileBuffer->getWord(index + 7);  // object_width
        int height = fileBuffer->getWord(index + 9);  // object_height

        if (width <= subPicture->width && height <= subPicture->height)
        {
            imgObj->fragmentList = QVector<ImageObjectFragment*>();
            info = new ImageObjectFragment;
            info->imageBufferOfs = index + 11;
            info->imagePacketSize = segment->size - ((index + 11) - segment->offset);
            imgObj->fragmentList.push_back(info);
            imgObj->bufferSize = info->imagePacketSize;
            imgObj->height = height;
            imgObj->width  = width;
            msg = QString("ID: %1, update: %2, seq: %3%4%5").arg(QString::number(objID))
                                                            .arg(QString::number(objVer))
                                                            .arg(first ? QString("first") : QString(""))
                                                            .arg((first && last) ? QString("/") : QString(""))
                                                            .arg(last ? QString("last") : QString(""));
            return true;
        }
        else
        {
            //TODO: print line
            return false;
        }
    }
    else
    {
        // object_data_fragment
        // skipped:
        //  16bit object_id
        //  8bit  object_version_number
        //  8bit  first_in_sequence (0x80), last_in_sequence (0x40), 6bits reserved
        info = new ImageObjectFragment;
        info->imageBufferOfs = index + 4;
        info->imagePacketSize = segment->size - (index + 4 - segment->offset);
        imgObj->fragmentList.push_back(info);
        imgObj->bufferSize += info->imagePacketSize;
        msg = QString("ID: %1, update: %2, seq: %3%4%5").arg(QString::number(objID))
                                                        .arg(QString::number(objVer))
                                                        .arg(first ? QString("first") : QString(""))
                                                        .arg((first && last) ? QString("/") : QString(""))
                                                        .arg(last ? QString("last") : QString(""));
        return false;
    }
}

void SupBD::parseWDS(SupSegment* segment, SubPictureBD* subPicture)
{
    int index = segment->offset;

    if (segment->size >= 10)
    {
        // skipped:
        // 8bit: number of windows (currently assumed 1, 0..2 is legal)
        // 8bit: window id (0..1)
        subPicture->xWinOfs   = fileBuffer->getWord(index+2);	// window_horizontal_position
        subPicture->yWinOfs   = fileBuffer->getWord(index+4);	// window_vertical_position
        subPicture->winWidth  = fileBuffer->getWord(index+6);	// window_width
        subPicture->winHeight = fileBuffer->getWord(index+8);	// window_height
    }
}

SupBD::CompositionState SupBD::getCompositionState(SupBD::SupSegment *segment)
{
    int type;
    type = fileBuffer->getByte(segment->offset + 7);
    switch (type)
    {
    case 0x00:
        return CompositionState::NORMAL;
    case 0x40:
        return CompositionState::ACQU_POINT;
    case 0x80:
        return CompositionState::EPOCH_START;
    case 0xC0:
        return CompositionState::EPOCH_CONTINUE;
    default:
        return CompositionState::INVALID;
    }
}

void SupBD::decode(SubPictureBD *subPicture)
{
    palette = decodePalette(subPicture);
    bitmap = decodeImage(subPicture, palette->getTransparentIndex());
    primaryColorIndex = bitmap->getPrimaryColorIndex(palette, subtitleProcessor->getAlphaThreshold());
}

Palette *SupBD::decodePalette(SubPictureBD *subPicture)
{
    bool fadeOut = false;
    int palIndex = 0;
    QVector<PaletteInfo*>* pl = subPicture->palettes.at(subPicture->getImgObj()->paletteID);
    if (pl == 0)
    {
        //TODO: error handling
        throw 10;
    }

    Palette* palette = new Palette(256, subtitleProcessor->usesBT601());
    // by definition, index 0xff is always completely transparent
    // also all entries must be fully transparent after initialization

    for (int j = 0; j < pl->size(); j++)
    {
        PaletteInfo* p = pl->at(j);
        int index = p->paletteOfs;
        for (int i = 0; i < p->paletteSize; ++i)
        {
            // each palette entry consists of 5 bytes
            palIndex = fileBuffer->getByte(index);
            int y = fileBuffer->getByte(++index);
            int cr, cb;
            if (subtitleProcessor->getSwapCrCb())
            {
                cb = fileBuffer->getByte(++index);
                cr = fileBuffer->getByte(++index);
            }
            else
            {
                cr = fileBuffer->getByte(++index);
                cb = fileBuffer->getByte(++index);
            }
            int alpha = fileBuffer->getByte(++index);

            int alphaOld = palette->getAlpha(palIndex);
            // avoid fading out
            if (alpha >= alphaOld)
            {
                if (alpha < subtitleProcessor->getAlphaCrop()) // to not mess with scaling algorithms, make transparent color black
                {
                    y = 16;
                    cr = 128;
                    cb = 128;
                }
                palette->setAlpha(palIndex, alpha);
            }
            else
            {
                fadeOut = true;
            }

            palette->setYCbCr(palIndex, y, cb, cr);
            index++;
        }
    }
    if (fadeOut)
    {
        //TODO: print warning
    }
    return palette;
}

Bitmap* SupBD::decodeImage(SubPictureBD *subPicture, int transparentIndex)
{
    int w = subPicture->getImageWidth();
    int h = subPicture->getImageHeight();
    // always decode image obj 0, start with first entry in fragmentlist
    ImageObjectFragment* fragment = subPicture->getImgObj()->fragmentList.at(0);
    long startOfs = fragment->imageBufferOfs;

    if (w > subPicture->width || h > subPicture->height)
    {
        //TODO: error handling
        throw 10;
    }

    Bitmap* bm = new Bitmap(w, h, transparentIndex);

    int b;
    int index = 0;
    int ofs = 0;
    int size = 0;
    int xpos = 0;

    // just for multi-packet support, copy all of the image data in one common buffer
    QVector<uchar> buf = QVector<uchar>(subPicture->getImgObj()->bufferSize);
    index = 0;
    for (int p = 0; p < subPicture->getImgObj()->fragmentList.size(); ++p)
    {
        // copy data of all packet to one common buffer
        fragment = subPicture->getImgObj()->fragmentList.at(p);
        for (int i = 0; i < fragment->imagePacketSize; ++i)
        {
            buf[index + i] = (uchar)fileBuffer->getByte(fragment->imageBufferOfs + i);
        }
        index += fragment->imagePacketSize;
    }

    index = 0;

    do
    {
        b = buf[index++] & 0xff;
        if (b == 0)
        {
            b = buf[index++] & 0xff;
            if (b == 0)
            {
                // next line
                ofs = (ofs / (w + (bm->getImg()->bytesPerLine() - w))) * (w + (bm->getImg()->bytesPerLine() - w));
                if (xpos < (w + (bm->getImg()->bytesPerLine() - w)))
                {
                    ofs += (w + (bm->getImg()->bytesPerLine() - w));
                }
                xpos = 0;
            }
            else
            {
                if ((b & 0xC0) == 0x40)
                {
                    // 00 4x xx -> xxx zeroes
                    size = ((b - 0x40) << 8) + (buf[index++] & 0xff);

                    uchar* pixels = bm->getImg()->bits();
                    for (int i = 0; i < size; ++i)
                    {
                        pixels[ofs++] = 0; /*(byte)b;*/
                    }
                    xpos += size;
                }
                else if ((b & 0xC0) == 0x80)
                {
                    // 00 8x yy -> x times value y
                    size = (b - 0x80);
                    b = buf[index++] & 0xff;

                    uchar* pixels = bm->getImg()->bits();
                    for (int i = 0; i < size; ++i)
                    {
                        pixels[ofs++] = (uchar)b;
                    }
                    xpos += size;
                }
                else if  ((b & 0xC0) != 0)
                {
                    // 00 cx yy zz -> xyy times value z
                    size = ((b - 0xC0) << 8)+(buf[index++] & 0xff);
                    b = buf[index++] & 0xff;

                    uchar* pixels = bm->getImg()->bits();
                    for (int i = 0; i < size; ++i)
                    {
                        pixels[ofs++] = (uchar)b;
                    }
                    xpos += size;
                }
                else
                {
                    uchar* pixels = bm->getImg()->bits();
                    // 00 xx -> xx times 0
                    for (int i = 0; i < b; ++i)
                    {
                        pixels[ofs++] = 0;
                    }
                    xpos += b;
                }
            }
        }
        else
        {
            uchar* pixels = bm->getImg()->bits();
            pixels[ofs++] = (uchar)b;
            xpos++;
        }
    } while (index < buf.size());

    return bm;
}

double SupBD::getFpsFromID(int id)
{
    switch (id)
    {
    case 0x20:
        return FPS_24HZ;
    case 0x30:
        return FPS_PAL;
    case 0x40:
        return FPS_NTSC;
    case 0x60:
        return FPS_PAL_I;
    case 0x70:
        return FPS_NTSC_I;
    default:
        return FPS_24P; // assume FPS_24P (also for FPS_23_975)
    }
}
