/*****************************************************************************
Copyright (c) 2005 - 2009, The Board of Trustees of the University of Illinois.
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the
  above copyright notice, this list of conditions
  and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the University of Illinois
  nor the names of its contributors may be used to
  endorse or promote products derived from this
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/*****************************************************************************
written by
   Yunhong Gu, last updated 11/25/2008
*****************************************************************************/


#include "log.h"
#include <time.h>
#include <string>
#include <cstring>
#include <iostream>

using namespace std;

SectorLog::SectorLog():
m_iLevel(1),
m_iDay(-1)
{
   pthread_mutex_init(&m_LogLock, NULL);
}

SectorLog::~SectorLog()
{
   pthread_mutex_destroy(&m_LogLock);
}

int SectorLog::init(const char* path)
{
   m_strLogPath = path;
   time_t t = time(NULL);
   tm date;
   gmtime_r(&t, &date);
   m_iDay = date.tm_mday;

   char fn[32];
   sprintf(fn, "%d.%d.%d.log", date.tm_mon + 1, date.tm_mday, date.tm_year + 1900);

   m_LogFile.open((m_strLogPath + "/" + fn).c_str(), ios::app);

   if (m_LogFile.bad() || m_LogFile.fail())
      return -1;

   return 0;
}

void SectorLog::close()
{
   m_LogFile.close();
}

void SectorLog::setLevel(const int level)
{
   if (level >= 0)
      m_iLevel = level;
}

void SectorLog::insert(const char* text, const int level)
{
   if (level > m_iLevel)
      return;

   pthread_mutex_lock(&m_LogLock);

   checkLogFile();

   time_t t = time(NULL);
   char ct[64];
   sprintf(ct, "%s", ctime(&t));
   ct[strlen(ct) - 1] = '\0';
   m_LogFile << ct << "\t" << text << endl;
   m_LogFile.flush();

   pthread_mutex_unlock(&m_LogLock);
}

void SectorLog::logUserActivity(const char* user, const char* ip, const char* cmd, const char* file, const char* res, const char* slave, const int level)
{
   if (level > m_iLevel)
      return;

   char* text = new char[128 + strlen(file)];
   sprintf(text, "user request => USER: %s  IP: %s  CMD: %s  FILE/DIR: %s  RESULT: %s  SLAVE: %s", user, ip, cmd, file, res, slave);
   insert(text, level);
   delete [] text;
}

void SectorLog::checkLogFile()
{
   time_t t = time(NULL);
   tm date;
   gmtime_r(&t, &date);
   if (date.tm_mday == m_iDay)
      return;

   m_iDay = date.tm_mday;

   char fn[32];
   sprintf(fn, "%d.%d.%d.log", date.tm_mon + 1, date.tm_mday, date.tm_year + 1900);

   m_LogFile.close();
   m_LogFile.open((m_strLogPath + "/" + fn).c_str(), ios::app);
}
