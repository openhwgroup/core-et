// Copyright (c) 2026 Ainekko
// SPDX-License-Identifier: Apache-2.0

//////////////////////////////////////////////////////////////////
// Submodule filelists (-f of submodule _rtl.f only)
//////////////////////////////////////////////////////////////////

-f $RTLROOT/libs/libs_rtl.f
-f $RTLROOT/shire/minion/vpu/txfma_rtl.f

//////////////////////////////////////////////////////////////////
// Include directories (+incdir+ and references to $RTLROOT only)
//////////////////////////////////////////////////////////////////

+incdir+$RTLROOT/inc

+incdir+$RTLROOT/shire/minion/intpipe/csr

//////////////////////////////////////////////////////////////////
// RTL files/directories (-v, -y, and references to $RTLROOT only)
//////////////////////////////////////////////////////////////////

-y $RTLROOT/shire/minion
-y $RTLROOT/shire/minion/dcache
-y $RTLROOT/shire/minion/frontend
-y $RTLROOT/shire/minion/intpipe
-y $RTLROOT/shire/minion/intpipe/csr
-y $RTLROOT/shire/minion/vpu
-y $RTLROOT/shire/minion/vpu/tima
-y $RTLROOT/shire/minion/vpu/trans_unit

