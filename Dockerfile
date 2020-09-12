FROM alpine:latest as stage

RUN apk update && \
    apk add glibmm-dev cairomm-dev gcc g++ cmake make

COPY . /build

WORKDIR /build

RUN cmake . && make

FROM alpine:latest
LABEL MAINTAINER="github/@rusq"

RUN apk update
RUN apk add libstdc++ cairomm glibmm msttcorefonts-installer

RUN update-ms-fonts && fc-cache -f

COPY --from=stage /build/src/dotprint /bin/dotprint

WORKDIR /work

ENTRYPOINT [ "/bin/dotprint" ]
CMD [ "--help" ]