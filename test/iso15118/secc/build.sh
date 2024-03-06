#!/bin/bash

g++ -g secc.cpp -o secc -std=c++17 -I./ -I../../include -I../../include/exi/cb -I../../../libfsm/include/ -L./libs -liso15118 -lcb_v2gtp -lcb_iso20 -lcb_exi_codec -lmbedtls -lmbedcrypto -lmbedx509
