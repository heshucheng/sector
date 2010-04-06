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
   Yunhong Gu, last updated 05/08/2009
*****************************************************************************/

#ifndef __SECTOR_FS_CACHE_H__
#define __SECTOR_FS_CACHE_H__

#include <index.h>
#include <string>
#include <map>
#include <list>
#include <pthread.h>

struct StatRec
{
   int m_iCount;		// number of reference
   int m_bChange;		// if the file has been changed
   int64_t m_llTimeStamp;	// time stamp
   int64_t m_llSize;		// file size
};

class StatCache
{
public:
   StatCache() {}
   virtual ~StatCache() {}

public:
   void insert(const std::string& path);
   void update(const std::string& path, const int64_t& ts, const int64_t& size);
   int stat(const std::string& path, SNode& attr);
   void remove(const std::string& path);

private:
   std::map<std::string, StatRec> m_mOpenedFiles;
   pthread_mutex_t m_Lock;
};

struct CacheBlock
{
   std::string m_strFileName;
   int64_t m_llOffset;
   int64_t m_llSize;
   int64_t m_llCreateTime;
   int64_t m_llLastAccessTime;
   int m_iAccessCount;
   char* m_pcBlock;
};

class ReadCache
{
public:
   ReadCache();
   virtual ~ReadCache();

public:
   int insert(char* block, const std::string& path, const int64_t& offset, const int64_t& size);
   int remove(const std::string& path);

   int read(const std::string& path, char* buf, const int64_t& offset, const int64_t& size);

private:
   int shrink();

private:
   std::map<std::string, std::list<CacheBlock> > m_mCacheBlocks;
   int64_t m_llCacheSize;
   int64_t m_llMaxCacheSize;
   int64_t m_llMaxCacheTime;
   pthread_mutex_t m_Lock;
};

#endif
