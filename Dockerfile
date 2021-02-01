FROM kurento/kurento-media-server
 
COPY . .

RUN apt-get update && \
    apt-get install -y --no-install-recommends build-essential cmake && \
    apt-get install -y --no-install-recommends kms-cmake-utils kms-core-dev && \
    apt-get install -y --no-install-recommends kms-elements-dev kms-filters-dev && \
    apt-get install -y --no-install-recommends libgstrtspserver-1.0-dev && \
    cd rtsp-to-rtp-module && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr && make && make install && \
    cd ../../rtsp-to-rtsp-module && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr && make && make install && \
    cd ../../file-to-rtsp-module && \
    mkdir build && cd build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=/usr && make && make install

EXPOSE 8888 7000-7100
