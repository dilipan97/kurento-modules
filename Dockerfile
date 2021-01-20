FROM kurento/kurento-media-server
 
COPY . .

RUN apt update && \
	apt install -y build-essential && \
	apt-get install -y manpages-dev && \
	apt-get install -y cmake && \
	apt-get install --no-install-recommends --yes kurento-media-server-dev && \
    apt-get install -y libgstrtspserver-1.0-dev && \
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