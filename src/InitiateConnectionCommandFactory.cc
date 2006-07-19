/* <!-- copyright */
/*
 * aria2 - a simple utility for downloading files faster
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/* copyright --> */
#include "InitiateConnectionCommandFactory.h"
#include "HttpInitiateConnectionCommand.h"
#include "FtpInitiateConnectionCommand.h"
#include "DlAbortEx.h"

Command* InitiateConnectionCommandFactory::createInitiateConnectionCommand(int cuid, Request* req, DownloadEngine* e) {
  if(req->getProtocol() == "http"
#ifdef ENABLE_SSL
     // for SSL
     || req->getProtocol() == "https"
#endif // ENABLE_SSL
     ) {
    return new HttpInitiateConnectionCommand(cuid, req, e);
  } else if(req->getProtocol() == "ftp") {
    return new FtpInitiateConnectionCommand(cuid, req, e);
  } else {
    // these protocols are not supported yet
    throw new DlAbortEx("%s is not supported yet.", req->getProtocol().c_str());
  }
}
