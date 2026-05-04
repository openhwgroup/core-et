/* Copyright (c) 2026 Ainekko          */
/* SPDX-License-Identifier: Apache-2.0 */


/* DCACHE */
namespace dcache_loads_stores {
#include "../../fmax_power/fmax_power_loads_and_stores/fmax_power_loads_and_stores.h"
}

/* VPU */
namespace vpu_txfma {
#include "../../power_vpu/kernel.h"
}
namespace vpu_tima8a32 {
#include "../../power_vpu/tima/workaround/kernel.h"
}
namespace vpu_bypass {
#include "../../power_vpu/bypass/workaround/kernel.h"
}
