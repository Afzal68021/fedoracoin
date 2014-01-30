FedoraCoin integration/staging tree
================================

http://www.fedoracoin.net

Copyright (c) 2009-2013 Bitcoin Developers

Copyright (c) 2011-2014 LiteCoin Developers

Copyright (c) 2013-2014 FedoraCoin Developers

What is FedoraCoin?
----------------

FedoraCoin is a clone of LiteCoin, using scrypt as a proof-of-work algorithm.

RPC port is 22888, P2P port is 22889 (44889 for testnet)

1 minute block target, difficulty readjusts every 10 blocks (10 minute difficulty readjustments)

Total of 500,000,000,000 coins. 

Special reward system: Random block rewards

1 - 100,000: 0 - 5,000,000 FedoraCoin Reward 

100,001 — 200,000: 0 - 2,500,000 FedoraCoin Reward 

200,001 — 300,000: 0 - 1,250,000 FedoraCoin Reward 

300,001 — 400,000: 0 - 625,000 FedoraCoin Reward 

400,001 — 500,000: 0 - 312,500 FedoraCoin Reward 

500,001 - 600,000: 0 - 156,250 FedoraCoin Reward

600,000+ — 10,000 Reward (flat)

For more information, as well as an immediately useable, binary version of
the FedoraCoin client sofware, see http://www.fedoracoin.net.

License
-------

FedoraCoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.

Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the FedoraCoin
development team members simply pulls it.

If it is a *more complicated or potentially controversial* change, then the patch
submitter will be asked to start a discussion (if they haven't already) on the
FedoraCoin IRC channel (#TIPS on freenode)

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see `doc/coding.txt`) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/fedoracoin/fedoracoin/tags) are created
regularly to indicate new official, stable release versions of FedoraCoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to
submit new unit tests for old code.

Unit tests for the core code are in `src/test/`. To compile and run them:

    cd src; make -f makefile.unix test

Unit tests for the GUI code are in `src/qt/test/`. To compile and run them:

    qmake BITCOIN_QT_TEST=1 -o Makefile.test bitcoin-qt.pro
    make -f Makefile.test
    ./fedoracoin-qt_test

