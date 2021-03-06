/*****************************************************************************
Copyright (c) 2005 - 2010, The Board of Trustees of the University of Illinois.
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
   Yunhong Gu, last updated 02/19/2010
*****************************************************************************/


#ifndef __SECTOR_ROUTING_H__
#define __SECTOR_ROUTING_H__

#include <vector>
#include <map>
#include <string>
#include "dhash.h"
#include <udt.h>
#include <sector.h>
#ifndef WIN32
   #include <pthread.h>
#endif

class Routing
{
public:
   Routing();
   ~Routing();

public:
   void init();

   int insert(const uint32_t& key, const Address& node);
   int remove(const uint32_t& key);

   int lookup(const std::string& path, Address& node);
   int lookup(const uint32_t& key, Address& node);

   int getEntityID(const std::string& path);

   int getRouterID(const uint32_t& key);
   int getRouterID(const Address& node);

   bool match(const uint32_t& cid, const uint32_t& key);
   bool match(const char* path, const uint32_t& key);

   int getPrimaryMaster(Address& node);

   int getNumOfMasters();

   void getListOfMasters(std::map<uint32_t, Address>& al);

private:
   std::vector<uint32_t> m_vFingerTable;
   std::map<uint32_t, Address> m_mAddressList;
   std::map<Address, uint32_t, AddrComp> m_mKeyList;

   int m_iKeySpace;

private:
   pthread_mutex_t m_Lock;
};

#endif
