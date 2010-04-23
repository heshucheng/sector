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
   Yunhong Gu, last updated 01/02/2010
*****************************************************************************/

#ifndef __SECURITY_H__
#define __SECURITY_H__

#include <ssltransport.h>
#include <map>
#include <vector>
#include <string>
#include <stdint.h>

struct Key
{
   char m_pcIP[64];
   int32_t m_iPort;
   int32_t m_iPass;		// pass phrase
   int32_t m_iValidPeriod;	// seconds
};

struct IPRange
{
   uint32_t m_uiIP;
   uint32_t m_uiMask;
};

class SUser
{
public:
   int serialize(const std::vector<std::string>& input, std::string& buf) const;

public:
   int32_t m_iID;

   std::string m_strName;
   std::string m_strPassword;
   std::vector<IPRange> m_vACL;

   std::vector<std::string> m_vstrReadList;
   std::vector<std::string> m_vstrWriteList;
   bool m_bExec;

   int64_t m_llQuota;
};

class SSource
{
public:
   virtual ~SSource() {}

public:
   virtual int loadACL(std::vector<IPRange>& acl, const void* src) = 0;
   virtual int loadUsers(std::map<std::string, SUser>& users, const void* src) = 0;

public:
   //virtual int passwd(const std::string& user, const std::string& password);
};

class SServer
{
public:
   SServer();
   ~SServer() {}

public:
   int init(const int& port, const char* cert, const char* key);
   int init_ASN1(const int& port, int clen, const unsigned char *cd, int klen, const unsigned char *kd);
   void close();

   int loadMasterACL(SSource* src, const void* param);
   int loadSlaveACL(SSource* src, const void* param);
   int loadShadowFile(SSource* src, const void* param);

   void run();

private:
   int32_t m_iKeySeed;
   int32_t generateKey();

private:
   struct Param
   {
      std::string ip;
      int port;
      SServer* sserver;
      SSLTransport* ssl; 
   };
   static void* process(void* p);

private:
   int m_iPort;
   SSLTransport m_SSL;

public:
   std::vector<IPRange> m_vMasterACL;
   std::vector<IPRange> m_vSlaveACL;
   std::map<std::string, SUser> m_mUsers;

private:
   static bool match(const std::vector<IPRange>& acl, const char* ip);
   static const SUser* match(const std::map<std::string, SUser>& users, const char* name, const char* password, const char* ip);
};

#endif
