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

#ifndef BICUBICFILTER_H
#define BICUBICFILTER_H

#include "filter.h"

class BiCubicFilter : public Filter
{
public:
    float getRadius() { return 2.0f; }
    float value(float value);

protected:
    static constexpr float a = -0.5;
    static constexpr float a_plus_2 = a + 2.0f;
    static constexpr float a_plus_3 = a + 3.0f;
    static constexpr float a_times_4 = a * 4;
    static constexpr float a_times_5 = a * 5;
    static constexpr float a_times_8 = a * 8;
};

#endif // BICUBICFILTER_H
