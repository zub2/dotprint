# What is it?
dotprint is a tool that can be used to convert text files that include escape sequences for Epson dot matrix printers ([ESC/P](https://files.support.epson.com/pdf/general/escp2ref.pdf)) into PDF files.

Nowadays you are not likely to come across such files often but they were common in the "bad old days" of DOS. Programs would often assume an "epson-compatible" dot matrix printer and would embed the escape sequences (for e.g. condensed or expanded font) into the output.

If you want to use such files now, converting them into PDF is quite useful. You can send them to others and you can print them out on other printers than just dot matrix printers. But please note this tool takes a naive approach and works well only for simple input files. :-(

So this might be useful to you if you are still running some DOS applications, perhaps in dosemu. With some scripting you can make the old DOS applications produce PDFs.

The input file encoding is expected to be a simple single byte encoding, as would be common in the DOS days. The conversion can either be done with iconv, e.g. for CP850:
```
dotprint -T CP850 --output myfile.pdf myfile.PRN
```
or a custom translation table can be specified, e.g. for the non-standard not-really-CP859:
```
dotprint -t tables/cp895.trans --output myfile.pdf myfile.PRN
```
The translation files are delivered with dotprint in the tables folder.

TODO: Specifiy an installation directory for the translation tables and reference this folder here.

# Compiling

First you need to install the required dependencies. These are:

* glibmm-2.4
* cairomm-1.0
* boost test (optional, needed to run tests)
* libiconv (at least on Linux a part of glibc and so it's not needed to be installed separately)

In Debian/Ubuntu you can get them via:

    apt install libglibmm-2.4-dev libcairomm-1.0-dev libboost-test-dev

CMake is used for the build. It's possible to configure and build the program by:

    cmake . && make

Prefix can be specified by adding `-DCMAKE_INSTALL_PREFIX=prefix` to the CMake invocation. Default is `/usr/local`.

# Installing
Run the `install` make target:

    make install

You can specify your own `DESTDIR`.

# Usage

You need to specify:

* the input file (text input with potential escape sequences)
* the output file (PDF)
* unless the text in the file is pure ascii, the encoding (-T or -t)

You can specify also the preprocessor using the `-P` option. It defaults to epson. The original idea was to potentially support other printer escape codes. But currently only `epson`, `simple` and `crlf` (with the latter two not processing any escapes).

A typical invocation of dotprint looks like this:

    dotprint input-file.txt -T CPnnn -o output-file.pdf

Run `dotprint -h` for a list of all the options.

# Docker
## Building Docker Container

This might be useful if you're using macOS and don't have development tools or C++ compiler, but have Docker.

To build the docker container, run:

    ./build-docker.sh

This will build `dotprint:latest` image based on `alpine:latest` with some preinstalled font (including the Courier New, which is used by default).

## Running in Docker

The easy way to run conversion is:

    ./convert.sh input.prn -o output.pdf

Docker container has `dotprint` as an entry point and any options specified on the command line are the commands are passed to the dotprint executable.

If you need to run this manually, you'd need to volume-mount the current directory to `/work` directory of the container, i.e.:

    docker run --rm -v `pwd`:/work dotprint \
        -P epson \
        tests/test4.ASCII.prn \
        -o output.pdf

# Licence

GNU GPL 3 or newer.
