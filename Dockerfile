FROM buildpack-deps:jessie

RUN echo 'deb http://download.opensuse.org/repositories/filesystems:/snapper/Debian_8.0/ /' >> /etc/apt/sources.list.d/snapper.list && \
    apt-get update && \
    apt-get install --force-yes -y snapper && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*
