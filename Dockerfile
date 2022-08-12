#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.
#

FROM perftool/compile:cpp

RUN echo "begin build" && \
    wget -q https://archive.apache.org/dist/pulsar/pulsar-2.10.1/DEB/apache-pulsar-client.deb && \
    wget -q https://archive.apache.org/dist/pulsar/pulsar-2.10.1/DEB/apache-pulsar-client-dev.deb && \
    apt-get install ./apache-pulsar-client.deb && \
    apt-get install ./apache-pulsar-client-dev.deb && \
    rm -f apache-pulsar-client.deb && \
    rm -f apache-pulsar-client-dev.deb && \
    echo "end build"

COPY . /opt/perf
WORKDIR /opt/perf

RUN cmake . && \
    make

CMD ["/usr/bin/dumb-init", "/opt/perf/perf-mq-consumer-cpp"]
