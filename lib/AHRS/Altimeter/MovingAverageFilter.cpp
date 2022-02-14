/*
https://github.com/sebnil/Moving-Avarage-Filter--Arduino-Library-

The MIT License (MIT)

Copyright (c) 2019 Sebastian Nilsson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

********

A moving average, also called rolling average, rolling mean or running average, is a type of finite impulse response filter (FIR) used to analyze a set of datum points by creating a series of averages of different subsets of the full data set.
https://en.wikipedia.org/wiki/Moving_average

*/
#include "MovingAverageFilter.h"

MovingAverageFilter::MovingAverageFilter(unsigned int newDataPointsCount)
{
  k = 0; //initialize so that we start to write at index 0
  if (newDataPointsCount < MAX_DATA_POINTS)
    dataPointsCount = newDataPointsCount;
  else
    dataPointsCount = MAX_DATA_POINTS;

  for (i = 0; i < dataPointsCount; i++)
  {
    values[i] = 0; // fill the array with 0's
  }
}

float MovingAverageFilter::process(float in)
{
  out = 0;

  values[k] = in;
  k = (k + 1) % dataPointsCount;

  for (i = 0; i < dataPointsCount; i++)
  {
    out += values[i];
  }

  return out / dataPointsCount;
}
