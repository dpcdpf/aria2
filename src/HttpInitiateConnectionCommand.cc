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
#include "HttpInitiateConnectionCommand.h"
#include "HttpRequestCommand.h"
#include "HttpProxyRequestCommand.h"
#include "Util.h"
#include "DlAbortEx.h"
#include "message.h"
#include "prefs.h"

HttpInitiateConnectionCommand::HttpInitiateConnectionCommand(int cuid,
							     Request* req,
							     DownloadEngine* e):AbstractCommand(cuid, req, e) {}

HttpInitiateConnectionCommand::~HttpInitiateConnectionCommand() {}

bool HttpInitiateConnectionCommand::executeInternal(Segment segment) {
  // socket->establishConnection(...);
  Command* command;
  if(useProxy()) {
    logger->info(MSG_CONNECTING_TO_SERVER, cuid,
		 e->option->get(PREF_HTTP_PROXY_HOST).c_str(),
		 e->option->getAsInt(PREF_HTTP_PROXY_PORT));
    socket->establishConnection(e->option->get(PREF_HTTP_PROXY_HOST),
				e->option->getAsInt(PREF_HTTP_PROXY_PORT));
    if(useProxyTunnel()) {
      command = new HttpProxyRequestCommand(cuid, req, e, socket);
    } else if(useProxyGet()) {
      command = new HttpRequestCommand(cuid, req, e, socket);
    } else {
      // TODO
      throw new DlAbortEx("ERROR");
    }
  } else {
    logger->info(MSG_CONNECTING_TO_SERVER, cuid, req->getHost().c_str(),
		 req->getPort());
    socket->establishConnection(req->getHost(), req->getPort());
    command = new HttpRequestCommand(cuid, req, e, socket);
  }
  e->commands.push_back(command);
  return true;
}

bool HttpInitiateConnectionCommand::useProxy() {
  return e->option->get(PREF_HTTP_PROXY_ENABLED) == V_TRUE;
}

bool HttpInitiateConnectionCommand::useProxyGet() {
  return e->option->get(PREF_HTTP_PROXY_METHOD) == V_GET;
}

bool HttpInitiateConnectionCommand::useProxyTunnel() {
  return e->option->get(PREF_HTTP_PROXY_METHOD) == V_TUNNEL;
}
