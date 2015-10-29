# tpm2_server

This repository implements a server which provides access to a real or
simulated TPM2 device over a socket interface.

A real TPM device could be connected over a USB-to-FTDI-to-SPI cable (for
instance http://goo.gl/VDlr8H). Software TPM2 implementation published at
https://goo.gl/r4qGVZ is used when running in simulation mode.

The `Makefile` by default assumes the following directory layout:

<pre>
.
|-- build/
|   `-- tpm2_server   <= build directory
|-- tpm2              <= TPM2 library (downloaded separately)
`-- tpm2_server       <= this server implementation
</pre>

Building the code on a recent Ubuntu host requires `gcc` toolchain and
`crypto` and `ftdi` libraries, other than that it is as simple as running
`make -j` in the `./tpm2_server` root directory.

The executable including the software TPM2 implementation is produced as
`../build/tpm2_server/ntpm`

Note that you don't need to build the TPM2 library separately, but if required
a standalone TPM2 library can be produced by running `make -j` in the
`../tpm2` directory, it is generated as `../tpm2/build/libtpm2.a`

==== Running the server =====

The server accepts the following command line options:
<pre>
  -d        - enable debug tracing
  -f NUM    - ftdi clock frequency  ## when using a TPM2 chip over USB
  -p NUM    - port number  ## port the server listens on, default value is 9833
  -s        - use simulator instead of the USB interface
</pre>

When you run `../build/tpm2_server/ntpm` with no command line arguments, it
will find and connect to a USB-to-FTDI-to-SPI adapter which expected to have a
TPM2 device attached (see below).

If you run it with `-s` command line argument, it starts the built-in TPM2
simulator instead.

Once initialized, the server listens on `localhost:9833` for incoming
connections. To talk to the TPM, you can send and receive binary data over the
socket. For example, the `TPM2_Startup` command is sent as
<pre>
80 01 00 00 00 0c 00 00 01 44 00 00
</pre>
and the expected response is
<pre>
80 01 00 00 00 0a 00 00 00 00
</pre>

==== Connecting an actual TPM2 device =========

If using the C232HM cable, the leads are connected to TPM pins as follows:

| Cable lead   | TPM pin |
|-------------:|:--------|
|  black       | GROUND  |
|  red         | VPP     |
|  green       | MISO    |
|  yellow      | MOSI    |
|  brown       | CS      |
|  orange      | CLK     |
|  gray        | RESET   |
