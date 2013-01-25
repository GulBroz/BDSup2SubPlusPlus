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

#ifndef IMAGEOBJECTFRAGMENT_H
#define IMAGEOBJECTFRAGMENT_H

#include <QtGlobal>

class ImageObjectFragment
{
public:
    ImageObjectFragment();
    ImageObjectFragment(const ImageObjectFragment& other) :
        packetSize(other.packetSize),
        bufferOfs(other.bufferOfs)
    {
        packetSize = other.packetSize;
        bufferOfs = other.bufferOfs;
    }

    int imagePacketSize() { return packetSize; }
    void setImagePacketSize(int imagePacketSize) { packetSize = imagePacketSize; }
    int imageBufferOffset() { return bufferOfs; }
    void setImageBufferOffset(int imageBufferOffset) { bufferOfs = imageBufferOffset; }

private:
    int packetSize = 0;
    quint64 bufferOfs = 0;
};

#endif // IMAGEOBJECTFRAGMENT_H
