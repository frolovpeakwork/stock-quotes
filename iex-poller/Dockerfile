FROM ubuntu

WORKDIR /projects/iex-poller

RUN apt update \
    && apt install -y \
        libcurl4-openssl-dev \
        libpq-dev \
        postgresql-server-dev-all \
    && rm -rf /var/lib/apt/lists/*

COPY ./cmake-build-release/IexPoller .

CMD ["./IexPoller"]
