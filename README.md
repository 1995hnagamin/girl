README
======

# Girl
[![Build Status](https://travis-ci.org/1995hnagamin/girl.svg?branch=master)](https://travis-ci.org/1995hnagamin/girl)

Girl is an online glossaries managner.

# Installation
## 1. make
Execute following commands:

    $ git clone https://github.com/1995hnagamin/girl.git
    $ cd girl
    $ mkdir build
    $ cd build
    $ cmake ../
    $ make
    $ make install

Default install path is /usr/local. To install them to another path,

    $ cmake -DCMAKE_INSTALL_PREFIX=/where/you/want/to/install/ ..

## 2. set environment variable
Append following line to ~/.\*shrc:

    export GIRLPATH=where/you/want/to/put/glossaries

# Usage

## View document

    girl <word>

For more information, see doc/.
