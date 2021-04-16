cmake_minimum_required(VERSION 3.19)
project(9315assignment02 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(9315assignment02
        bits.c
        bits.h
        bsig.c
        bsig.h
        defs.h
        hash.c
        hash.h
        insert.c
        page.c
        page.h
        psig.c
        psig.h
        query.c
        query.h
        reln.c
        reln.h
        tsig.c
        tsig.h
        tuple.c
        tuple.h
        util.c
        util.h
       )
