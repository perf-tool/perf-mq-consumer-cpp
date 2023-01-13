/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <sstream>
#include <pulsar/Client.h>
#include "env_util.cpp"

using namespace std;
using namespace pulsar;

static string pulsarHost = EnvUtil::getEnv("PULSAR_HOST", "localhost");
static int pulsarPort = EnvUtil::getEnv("PULSAR_PORT", 6650);
static string pulsarAuthToken = EnvUtil::getEnv("PULSAR_AUTH_TOKEN", "");

class Pulsar {
public:
    [[noreturn]] static void start() {
        ClientConfiguration configuration = ClientConfiguration();
        if (pulsarAuthToken.empty()) {
            configuration.setAuth(AuthToken::createWithToken(pulsarAuthToken));
        }
        Client client(getPulsarUrl(), configuration);
        Consumer consumer;
        ConsumerConfiguration config;
        Result result = client.subscribe(getenv("PULSAR_TOPIC"), getenv("PULSAR_SUBSCRIPTION_NAME"), config, consumer);
        if (result != ResultOk) {
            std::cout << "create consumer failed" << std::endl;
        } else {
            std::cout << "create consumer success" << std::endl;
        }
        Message msg;
        for (;;) {
            consumer.receive(msg);
            consumer.acknowledge(msg);
        }
    }

private:
    static string getPulsarUrl() {
        std::ostringstream stringStream;
        stringStream << "pulsar://" << pulsarHost << ":" << pulsarPort;
        return stringStream.str();
    }
};
