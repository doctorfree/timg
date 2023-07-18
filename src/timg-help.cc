// -*- mode: c++; c-basic-offset: 4; indent-tabs-mode: nil; -*-
// (c) 2023 Henner Zeller <h.zeller@acm.org>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation version 2.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://gnu.org/licenses/gpl-2.0.txt>

#include "timg-help.h"

#include <libdeflate.h>

// Currently, this is populated manually. TODO: add to CMake as a generator,
// and don't include timg-manpage.inc in the repository.
// Requires pandoc to be installed to get the freshest.
// Execute the following in the build/ dir.
#if 0
man -Tascii man/timg.1 | gzip -9       \
    | od -tx1 -Anone | sed 's/ /\\x/g' \
    | awk 'BEGIN { printf("/* auto generated */\nstatic constexpr char kGzippedManpage[] ="); } \
          { printf("\n    \"%s\"", $0); } \
          END { printf(";\n"); }' \
    > ../src/timg-manpage.inc
#endif

#include "timg-manpage.inc"

void InvokeHelpPager() {
    libdeflate_decompressor *const decompress = libdeflate_alloc_decompressor();

    // TODO(hzeller): include in the inc-file the original size.
    char uncompressed[1 << 16];
    size_t uncompressed_size = 0;
    libdeflate_result result = libdeflate_gzip_decompress(
        decompress, kGzippedManpage, sizeof(kGzippedManpage), uncompressed,
        sizeof(uncompressed), &uncompressed_size);

    libdeflate_free_decompressor(decompress);
    if (result == LIBDEFLATE_SUCCESS) {
        FILE *myout = popen("${PAGER:-less}", "w");
        fwrite(uncompressed, 1, uncompressed_size, myout);
        pclose(myout);
    }
}
