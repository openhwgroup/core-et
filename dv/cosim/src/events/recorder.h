/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */

#ifndef _RECORDER_H_
#define _RECORDER_H_

#include <fstream>
#include <string>

#include <cereal/archives/binary.hpp>

#include "cosim_evt.h"

#include "lz4_stream.h"

struct recorder : cosim_evt::handler {
public:
    recorder(const std::string& fname)
        : ofs_(fname, std::ios::binary)
        , lz4_(ofs_)
        , save_(lz4_)
    {
    }

#define DECL_CALLBACK(EVT) virtual void on(const cosim_evt::EVT##_t* evt);
    COSIM_EVT_FOREACH(DECL_CALLBACK)
#undef DECL_CALLBACK

private:
    std::ofstream ofs_;
    lz4_stream::basic_ostream<4096> lz4_;
    cereal::BinaryOutputArchive save_;
};

#endif // _RECORDER_H_
