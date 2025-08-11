
# Stub Dockerfile. You must provide the legacy avr32 toolchain and ASF.
# One approach:
#  - Build from a Debian base
#  - Install the legacy Atmel AVR32 toolchain and ASF 3.45.0 automatically
#
FROM debian:stable-slim
RUN apt-get update && apt-get install -y wget unzip tar make git dfu-programmer build-essential python2 && rm -rf /var/lib/apt/lists/*

# Install AVR32 toolchain (Atmel's last release, 3.4.2)
RUN wget -O /tmp/avr32-toolchain.tar.gz "https://ww1.microchip.com/downloads/en/DeviceDoc/avr32-gnu-toolchain-3.4.2.435-linux.any.x86_64.tar.gz" \
    && mkdir -p /opt/avr32 \
    && tar -xzvf /tmp/avr32-toolchain.tar.gz -C /opt/avr32 --strip-components=1 \
    && rm /tmp/avr32-toolchain.tar.gz

# Install ASF 3.45.0
RUN wget -O /tmp/asf-standalone-archive-3.45.0.88.zip "https://ww1.microchip.com/downloads/en/DeviceDoc/asf-standalone-archive-3.45.0.88.zip" \
    && unzip /tmp/asf-standalone-archive-3.45.0.88.zip -d /opt/ \
    && mv /opt/xdk-asf-3.45.0 /opt/asf-3.45.0 \
    && rm /tmp/asf-standalone-archive-3.45.0.88.zip

ENV PATH="/opt/avr32/bin:${PATH}"
ENV ASF_PATH="/opt/asf-3.45.0"
WORKDIR /work

CMD ["make"]
