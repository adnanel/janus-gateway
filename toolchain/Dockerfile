FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive

# docker build arguments
ARG BUILD_SRC="/usr/local/src"
ARG JANUS_REPO="https://github.com/adnanel/janus-gateway.git"
ARG JANUS_BRANCH="stun_alt"
ARG JANUS_COMMIT="b98e3bb91bd728ce21f6fd56519a303f2775f755"
ARG LIBNICE_VERSION="master"
ARG LIBWEBSOCKETS_VERSION="v4.3.2"
ARG JANUS_WITH_POSTPROCESSING="0"
ARG JANUS_WITH_BORINGSSL="0"
ARG JANUS_WITH_DOCS="0"
ARG JANUS_WITH_REST="1"
ARG JANUS_WITH_DATACHANNELS="1"
ARG JANUS_WITH_WEBSOCKETS="1"
ARG JANUS_WITH_MQTT="0"
ARG JANUS_WITH_PFUNIX="0"
ARG JANUS_WITH_RABBITMQ="0"
ARG ENABLE_ADDRESS_SANITIZER="false"
ARG JANUS_CONFIG_DEPS="\
    --prefix=/opt/janus \
    "
ARG JANUS_CONFIG_OPTIONS="\
    "
ARG JANUS_BUILD_DEPS_DEV="\
    libcurl4-openssl-dev \
    libjansson-dev \
    libssl-dev \
    libsofia-sip-ua-dev \
    libglib2.0-dev \
    libopus-dev \
    libogg-dev \
    libasan5 \
    pkg-config \
    gtk-doc-tools \
    libconfig-dev \
    "
ARG JANUS_BUILD_DEPS_EXT="\
    libavutil-dev \
    libavcodec-dev \
    libavformat-dev \
    gengetopt \
    libtool \
    automake \
    git-core \
    build-essential \
    cmake \
    ca-certificates \
    curl \
    "
ARG JANUS_SANITIZER_OPTIONS="\
    -O1 \
    -g3 \
    -ggdb3 \
    -fno-omit-frame-pointer \
    -fsanitize=address \
    -fno-sanitize-recover=all \
    -fsanitize-address-use-after-scope \
    "


RUN apt update && apt install -y build-essential pkg-config git gcc g++ flex bison make autoconf \
    libssl-dev libcurl4-openssl-dev libxml2-dev libpcre3-dev \
    libmongoc-1.0-0 libmongoc-dev libbson-1.0-0 cmake libssl-dev libsasl2-dev


RUN DEBIAN_FRONTEND="noninteractive" apt-get update && apt-get -y install tzdata

RUN apt-get update \
  && apt-get install -y build-essential \
      gcc \
      g++ \
      gdb \
      clang \
      make \
      ninja-build \
      cmake \
      autoconf \
      automake \
      libtool \
      valgrind \
      locales-all \
      dos2unix \
      rsync \
      tar \
      python \
      python-dev

# Install python and pip
RUN apt-get update && apt-get install -y python3 python3-pip ninja-build
RUN pip3 install requests
RUN pip3 install meson

RUN apt-get install nftables libspeexdsp-dev -y
RUN git config --global http.sslVerify false

RUN \
# init build env & install apt deps \
    export JANUS_WITH_POSTPROCESSING="${JANUS_WITH_POSTPROCESSING}" \
    && export JANUS_WITH_BORINGSSL="${JANUS_WITH_BORINGSSL}"\
    && export JANUS_WITH_DOCS="${JANUS_WITH_DOCS}"\
    && export JANUS_WITH_REST="${JANUS_WITH_REST}"\
    && export JANUS_WITH_DATACHANNELS="${JANUS_WITH_DATACHANNELS}"\
    && export JANUS_WITH_WEBSOCKETS="${JANUS_WITH_WEBSOCKETS}"\
    && export JANUS_WITH_MQTT="${JANUS_WITH_MQTT}"\
    && export JANUS_WITH_PFUNIX="${JANUS_WITH_PFUNIX}"\
    && export JANUS_WITH_RABBITMQ="${JANUS_WITH_RABBITMQ}"\
    && export JANUS_WITH_FREESWITCH_PATCH="${JANUS_WITH_FREESWITCH_PATCH}"\
    && export JANUS_BUILD_DEPS_DEV="${JANUS_BUILD_DEPS_DEV}"\
    && export JANUS_CONFIG_OPTIONS="${JANUS_CONFIG_OPTIONS}"\
    && export JANUS_SANITIZER_OPTIONS="${JANUS_SANITIZER_OPTIONS}"\
    && export ENABLE_ADDRESS_SANITIZER="${ENABLE_ADDRESS_SANITIZER}"\
    && if [ $JANUS_WITH_POSTPROCESSING = "1" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --enable-post-processing"; fi \
    && if [ $JANUS_WITH_BORINGSSL = "1" ]; then export JANUS_BUILD_DEPS_DEV="$JANUS_BUILD_DEPS_DEV golang-go" && export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --enable-boringssl --enable-dtls-settimeout"; fi \
    && if [ $JANUS_WITH_DOCS = "1" ]; then export JANUS_BUILD_DEPS_DEV="$JANUS_BUILD_DEPS_DEV doxygen graphviz" && export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --enable-docs"; fi \
    && if [ $JANUS_WITH_REST = "1" ]; then export JANUS_BUILD_DEPS_DEV="$JANUS_BUILD_DEPS_DEV libmicrohttpd-dev"; else export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-rest"; fi \
    && if [ $JANUS_WITH_DATACHANNELS = "0" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-data-channels"; fi \
    && if [ $JANUS_WITH_WEBSOCKETS = "0" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-websockets"; fi \
    && if [ $JANUS_WITH_MQTT = "0" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-mqtt"; fi \
    && if [ $JANUS_WITH_PFUNIX = "0" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-unix-sockets"; fi \
    && if [ $JANUS_WITH_RABBITMQ = "0" ]; then export JANUS_CONFIG_OPTIONS="$JANUS_CONFIG_OPTIONS --disable-rabbitmq"; fi \
    && /usr/sbin/groupadd -r janus && /usr/sbin/useradd -r -g janus janus \
    && DEBIAN_FRONTEND=noninteractive apt-get update \
    && DEBIAN_FRONTEND=noninteractive apt-get -y --no-install-recommends install $JANUS_BUILD_DEPS_DEV ${JANUS_BUILD_DEPS_EXT} \
# build libsrtp
    && curl -fSL https://github.com/cisco/libsrtp/archive/v2.1.0.tar.gz -o ${BUILD_SRC}/v2.1.0.tar.gz \
    && tar xzf ${BUILD_SRC}/v2.1.0.tar.gz -C ${BUILD_SRC} \
    && cd ${BUILD_SRC}/libsrtp-2.1.0 \
    && ./configure --prefix=/usr --enable-openssl \
    && make shared_library \
    && make install \
# build boringssl
    && if [ $JANUS_WITH_BORINGSSL = "1" ]; then git clone https://boringssl.googlesource.com/boringssl ${BUILD_SRC}/boringssl \
    && cd ${BUILD_SRC}/boringssl \
    && sed -i s/" -Werror"//g CMakeLists.txt \
    && mkdir -p ${BUILD_SRC}/boringssl/build \
    && cd ${BUILD_SRC}/boringssl/build \
    && cmake -DCMAKE_CXX_FLAGS="-lrt" .. \
    && make \
    && mkdir -p /opt/boringssl \
    && cp -R ${BUILD_SRC}/boringssl/include /opt/boringssl/ \
    && mkdir -p /opt/boringssl/lib \
    && cp ${BUILD_SRC}/boringssl/build/ssl/libssl.a /opt/boringssl/lib/ \
    && cp ${BUILD_SRC}/boringssl/build/crypto/libcrypto.a /opt/boringssl/lib/ \
    ; fi \
# build usrsctp
    && if [ $JANUS_WITH_DATACHANNELS = "1" ]; then git clone https://github.com/sctplab/usrsctp ${BUILD_SRC}/usrsctp \
    && cd ${BUILD_SRC}/usrsctp \
    && ./bootstrap \
    && ./configure --prefix=/usr \
    && make \
    && make install \
    ; fi \
# build libwebsockets
    && if [ $JANUS_WITH_WEBSOCKETS = "1" ]; then git clone https://github.com/warmcat/libwebsockets.git ${BUILD_SRC}/libwebsockets \
    && cd ${BUILD_SRC}/libwebsockets \
    && git checkout $LIBWEBSOCKETS_VERSION \
    && mkdir ${BUILD_SRC}/libwebsockets/build \
    && cd ${BUILD_SRC}/libwebsockets/build \
    && cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_C_FLAGS="-fpic" .. \
    && make \
    && make install \
    ; fi \
# build paho.mqtt.c
    && if [ $JANUS_WITH_MQTT = "1" ]; then git clone https://github.com/eclipse/paho.mqtt.c.git ${BUILD_SRC}/paho.mqtt.c \
    && cd ${BUILD_SRC}/paho.mqtt.c \
    && make \
    && make install \
    ; fi \
# build rabbitmq-c
    && if [ $JANUS_WITH_RABBITMQ = "1" ]; then git clone https://github.com/alanxz/rabbitmq-c ${BUILD_SRC}/rabbitmq-c \
    && cd ${BUILD_SRC}/rabbitmq-c \
    && git submodule init \
    && git submodule update \
    && autoreconf -i \
    && ./configure --prefix=/usr \
    && make \
    && make install \
    ; fi \
# build libnice
    && apt-get remove -y libnice-dev libnice10 \
    && git clone https://gitlab.freedesktop.org/libnice/libnice.git ${BUILD_SRC}/libnice \
    && cd ${BUILD_SRC}/libnice \
    && git checkout $LIBNICE_VERSION \
    && meson build \
    && ninja -C build \
    && meson test -C build \
    && ninja -C build install

RUN apt -y install gdb
ARG UID=1000
RUN useradd -m -u ${UID} -s /bin/bash builder
USER builder
