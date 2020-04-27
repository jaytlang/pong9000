# Pong 9000

Pong 9000 is a game runtime for the ESP32 and similar embedded devices. It provides a flexible code-sharing framework and a basic C runtime that enables easy-to-implement, real-time multiplayer games.

## The compiler
Pong 9000 uses the FPSR compiler, which compiles an integer-only subset of C into bytecode and executes it on an onboard virtual machine. This eliminates the need to re-flash an ESP32 every time you want to play a new game, and enables code sharing at scale when coupled with a game repository. As evidenced by the Game Browser - an app-store like FPSR program that allows users to install new games - this repository is open-access and allows anyone to efficiently compile games wherever they are.

The source code for the compiler is located in the top-level directory of this repository. It is optimized aggressively for size without compromising readability, and - inspired by the *c4* family of compilers - is fully self-hosting. The compiler permits a subset of C with the following features:
* characters, integers, and arbitrary-depth pointers
* enums, plus indexing and typical C arithmetic / control flow operations
* recursive function calls, stack-based local declarations
* co-opt of the host heap for array operations
* a broadly supported library, located in `libfpsr.h`, which offers a host of methods to access the host ESP32's functionality.

The document at `misc/C-restrictions.txt` provides more information about what isn't supported, and the library files are well documented re. what *is* supported.

## Structure of this repo
See the `games/` directory for officially supported games up on the database - the README there documents gameplay in further detail and serves as a nice checklist to mainline new software.

In the `server/` directory, you'll find documentation for the database API, as well as stripped source code for a .NET web app that enables easy posting of new games. Currently, it resides [here](https://web.mit.edu/jaytlang/postcode/).

Other files in the top level directory include legacy `testbench` files used in testing the compiler before it was connected to the internet, as well as the bootstrap code essential for onlining FPSR.
