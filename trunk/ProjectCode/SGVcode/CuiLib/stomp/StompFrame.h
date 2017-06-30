/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_
#define _ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_

#include <string>
#include <string.h>
#include <map>

#include <sstream>
#include <string.h>
#include <vector>
using namespace std;

#include "StompCommandConstants.h"
    /**
     * A Stomp-level message frame that encloses all messages to and from the broker.
     */
    class  StompFrame {
    private:

        // String Name of this command.
        std::string command;

        // Properties of the Stomp Message
         map<string, string> properties;

        // Byte data of Body.
        std::vector<unsigned char> body;

    public:

        /**
         * Default constructor.
         */
        StompFrame();

        /**
         * Destruction.
         */
        virtual ~StompFrame();

        static string GetConnectCmd();

		void setCommand(const std::string cmd_p);
		void setProperty(string key_p,string value_p);
		string GetCmdString();

    };



#endif /*_ACTIVEMQ_WIREFORMAT_STOMP_STOMPFRAMEWRAPPER_H_*/
