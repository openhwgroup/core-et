
#include "fpu_types.h"
#include "platform.h"
#include "internals.h"
#include "specialize.h"

#include <utility>

#ifdef FPU_DEBUG
#undef FPU_DEBUG
#endif

#ifdef FPU_DEBUG
#include "debug.h"
#endif


static uint_fast32_t softfloat_propagateNaNF32UI(
    uint_fast32_t uiA, uint_fast32_t uiB, uint_fast32_t uiC )
{
    if ( softfloat_isSigNaNF32UI( uiA ) || softfloat_isSigNaNF32UI( uiB ) ||
         softfloat_isSigNaNF32UI( uiC ) ) {
        softfloat_raiseFlags( softfloat_flag_invalid );
    }
    return defaultNaNF32UI;
}


static uint_fast32_t f16_mulExt( uint_fast16_t uiA, uint_fast16_t uiB )
{
    bool signA;
    int_fast8_t expA;
    uint_fast16_t sigA;
    bool signB;
    int_fast8_t expB;
    uint_fast16_t sigB;
    bool signZ;
    uint_fast16_t magBits;
    struct exp8_sig16 normExpSig;
    int_fast16_t expZ;
    uint_fast32_t sigZ;
    uint_fast32_t uiZ;
    struct commonNaN commonNaN __attribute__((unused));

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signA = signF16UI( uiA );
    expA  = expF16UI( uiA );
    sigA  = fracF16UI( uiA );
    signB = signF16UI( uiB );
    expB  = expF16UI( uiB );
    sigB  = fracF16UI( uiB );
    signZ = signA ^ signB;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0x1F ) {
        if ( sigA || ((expB == 0x1F) && sigB) ) goto propagateNaN;
        magBits = expB | sigB;
        goto infArg;
    }
    if ( expB == 0x1F ) {
        if ( sigB ) goto propagateNaN;
        magBits = expA | sigA;
        goto infArg;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( ! expA ) {
        if ( ! sigA ) goto zero;
        normExpSig = softfloat_normSubnormalF16Sig( sigA );
        expA = normExpSig.exp;
        sigA = normExpSig.sig;
    }
    if ( ! expB ) {
        if ( ! sigB ) goto zero;
        normExpSig = softfloat_normSubnormalF16Sig( sigB );
        expB = normExpSig.exp;
        sigB = normExpSig.sig;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expZ = expA + expB - 0xF + 0x70;
    sigA = sigA | 0x0400;
    sigB = sigB | 0x0400;
    sigZ = (uint_fast32_t) sigA * sigB;
#ifdef FPU_DEBUG
    std::cout << "a: " << Float16(signA,expA,sigA) << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
    std::cout << "b: " << Float16(signB,expB,sigB) << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
#endif
    uiZ = packToF32UI( signZ, expZ, sigZ );
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF16UI( uiA, uiB );
    softfloat_f16UIToCommonNaN( uiZ, &commonNaN );
    uiZ = softfloat_commonNaNToF32UI( &commonNaN );
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 infArg:
    if ( ! magBits ) {
        softfloat_raiseFlags( softfloat_flag_invalid );
        uiZ = defaultNaNF32UI;
    } else {
        uiZ = packToF32UI( signZ, 0xFF, 0 );
    }
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 zero:
    uiZ = packToF32UI( signZ, 0, 0 );
 uiZ:
#ifdef FPU_DEBUG
    {
        union ui32_f32 uZ;
        uZ.ui = uiZ;
        std::cout << "p: " << uZ.f << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
    }
#endif
    return uiZ;
}


static float32_t f32_add2( uint_fast32_t uiA, uint_fast32_t uiB )
{
    bool signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    bool signB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    int_fast8_t subB;
    int_fast16_t expDiff;
    union ui32_f32 uZ;
    uint_fast32_t uiZ;
    bool signZ;
    int_fast16_t expZ;
    uint_fast32_t sigZ;
    int_fast8_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    signB = signF32UI( uiB );
    expB  = expF32UI( uiB );
    sigB  = fracF32UI( uiB );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sigA <<= 7;
    sigB <<= 7;
#ifdef FPU_DEBUG
    std::cout << "a_orig: " << Float32<4>(signA,expA,sigA) << '\n';
    std::cout << "b_orig: " << Float32<4>(signB,expB,sigB) << '\n';
#endif
    if ( ( expA < expB ) || ( expA == expB && sigA < sigB ) ) {
        std::swap(signA, signB);
        std::swap(expA, expB);
        std::swap(sigA, sigB);
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0xFF ) {
        if ( sigA ) goto propagateNaN;
        if ( expB == 0xFF ) {
            if ( sigB ) goto propagateNaN;
            if ( signA ^ signB ) goto generateNaN;
        }
        uiZ = packToF32UI( signA, 0xFF, 0 );
        goto uiZ;
    }
#ifdef FPU_DEBUG
    std::cout << "H_norm: " << Float32<4>(signA,expA,sigA) << '\n';
    std::cout << "M_norm: " << Float32<4>(signB,expB,sigB) << '\n';
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiff = expA - expB;
#ifdef FPU_DEBUG
    std::cout << "H_shft: " << Float32<4>(signA,expA,sigA) << " (shft: 0)\n";
#endif
    if ( expDiff ) {
        sigB = softfloat_shiftRightJam32( sigB, expDiff );
        sigB = (sigB & ~2) | ((sigB & 2) >> 1);
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    subB = signA ^ signB;
#ifdef FPU_DEBUG
    std::cout << "M_shft: " << Float32<4>(signB,expA,sigB) << " (shft: " << expDiff << ")\n";
#endif
    signZ = signA;
    expZ = expA;
    sigZ = sigA + (subB ? -sigB : sigB);
#ifdef FPU_DEBUG
    std::cout << "H_add : " << Float32<4>(signA,expZ,sigA) << '\n';
    std::cout << "M_add : " << Float32<4>(signB,expZ,(subB ? -sigB : sigB)) << " (inv: " << (!!subB) << ")\n";
    std::cout << "z_sum : " << Float32<4>(signZ,expZ,sigZ) << "\n\n";
#endif
    if ( sigZ >= 0x80000000 ) {
        signZ = !signZ;
        sigZ = (-sigZ & ~2);
#ifdef FPU_DEBUG
        std::cout << "z_neg : " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( sigZ >= 0x10000000 ) {
        expZ += 1;
        sigZ = ((sigZ >> 1) & ~2) | ((sigZ >> 2) & 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj1: " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( sigZ >= 0x10000000 ) {
        expZ += 1;
        sigZ = ((sigZ >> 1) & ~2) | ((sigZ >> 2) & 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj2: " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( !sigZ ) {
        if ( subB ) {
            uiZ =
                packToF32UI(
                    (softfloat_roundingMode == softfloat_round_min), 0, 0 );
            goto uiZ;
        } else if ( !expDiff ) {
            uiZ = packToF32UI(signZ, expZ, 0);
            goto uiZ;
        }
    }
    sigZ = ((sigZ & ~1) << 2) | (sigZ & 1);
    if ( sigZ < 0x40000000 ) {
        --expZ;
        sigZ = ((sigZ & ~1) << 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj3: " << Float32<7>(signZ,expZ,sigZ) << '\n';
#endif
    }
    shiftDist = softfloat_countLeadingZeros32( sigZ ) - 1;
    expZ -= shiftDist;
    sigZ = ((sigZ & ~1) << shiftDist) | (sigZ & 1);
#ifdef FPU_DEBUG
    std::cout << "z_adj4: " << Float32<7>(signZ,expZ,sigZ) << '\n';
    {
        float32_t fZ;
        fZ = softfloat_roundPackToF32( signZ, expZ, sigZ );
        std::cout << "z_rslt: " << fZ << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
    }
#endif
    return softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 generateNaN:
    softfloat_raiseFlags( softfloat_flag_invalid );
    uiZ = defaultNaNF32UI;
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB );
 uiZ:
    uZ.ui = uiZ;
#ifdef FPU_DEBUG
    std::cout << "z_rslt: " << uZ.f << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
#endif
    return uZ.f;
}


static float32_t f32_add3( uint_fast32_t uiA, uint_fast32_t uiB, uint_fast32_t uiC )
{
    bool signA;
    int_fast16_t expA;
    uint_fast32_t sigA;
    bool signB;
    int_fast16_t expB;
    uint_fast32_t sigB;
    bool signC;
    int_fast16_t expC;
    uint_fast32_t sigC;
    int_fast8_t subB;
    int_fast8_t subC;
    int_fast16_t expDiffB;
    int_fast16_t expDiffC;
    union ui32_f32 uZ;
    uint_fast32_t uiZ;
    bool signZ;
    int_fast16_t expZ;
    uint_fast32_t sigZ;
    int_fast8_t shiftDist;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    signA = signF32UI( uiA );
    expA  = expF32UI( uiA );
    sigA  = fracF32UI( uiA );
    signB = signF32UI( uiB );
    expB  = expF32UI( uiB );
    sigB  = fracF32UI( uiB );
    signC = signF32UI( uiC );
    expC  = expF32UI( uiC );
    sigC  = fracF32UI( uiC );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sigA <<= 3;
    sigB <<= 3;
    if ( expC ) {
        if ( expC != 0xFF ) sigC |= 0x00800000;
    } else {
        expC = ( sigC != 0 );
    }
#ifdef FPU_DEBUG
    std::cout << "a_orig: " << Float32<0>(signA,expA,sigA) << '\n';
    std::cout << "b_orig: " << Float32<0>(signB,expB,sigB) << '\n';
    std::cout << "c_orig: " << Float32<0>(signC,expC,sigC) << "\n\n";
#endif
    if ( ( expA < expB ) || ( expA == expB && sigA < sigB ) ) {
        std::swap(signA, signB);
        std::swap(expA, expB);
        std::swap(sigA, sigB);
    }
    if ( ( expA < expC ) || ( expA == expC && sigA < sigC ) ) {
        std::swap(signA, signC);
        std::swap(expA, expC);
        std::swap(sigA, sigC);
    }
    if ( ( expB < expC ) || ( expB == expC && sigB < sigC ) ) {
        std::swap(signB, signC);
        std::swap(expB, expC);
        std::swap(sigB, sigC);
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    if ( expA == 0xFF ) {
        if ( expB == 0xFF ) {
            if ( !sigA && !sigB && (signA ^ signB) ) goto generateNaN;
            if ( expC == 0xFF ) {
                if ( !sigA && !sigC && (signA ^ signC) ) goto generateNaN;
                if ( !sigB && !sigC && (signB ^ signC) ) goto generateNaN;
                if ( sigC ) goto propagateNaN;
            }
            if ( sigB ) goto propagateNaN;
        }
        if ( sigA ) goto propagateNaN;
        uiZ = packToF32UI( signA, 0xFF, 0 );
        goto uiZ;
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    sigA <<= 4;
    sigB <<= 4;
    sigC <<= 4;
#ifdef FPU_DEBUG
    std::cout << "H_norm: " << Float32<4>(signA,expA,sigA) << '\n';
    std::cout << "M_norm: " << Float32<4>(signB,expB,sigB) << '\n';
    std::cout << "L_norm: " << Float32<4>(signC,expC,sigC) << "\n\n";
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    expDiffB = expA - expB;
    expDiffC = expA - expC;
#ifdef FPU_DEBUG
    std::cout << "H_shft: " << Float32<4>(signA,expA,sigA) << " (shft: 0)\n";
#endif
    if ( expDiffB ) {
        sigB = softfloat_shiftRightJam32( sigB, expDiffB );
        sigB = (sigB & ~2) | ((sigB & 2) >> 1);
    }
    if ( expDiffC ) {
        sigC = softfloat_shiftRightJam32( sigC, expDiffC );
        sigC = (sigC & ~2) | ((sigC & 2) >> 1);
    }
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    subB = signA ^ signB;
    subC = signA ^ signC;
#ifdef FPU_DEBUG
    std::cout << "M_shft: " << Float32<4>(signB,expA,sigB) << " (shft: " << expDiffB << ")\n";
    std::cout << "L_shft: " << Float32<4>(signC,expA,sigC) << " (shft: " << expDiffC << ")\n\n";
#endif
    signZ = signA;
    expZ = expA;
    sigZ = sigA + (subB ? (-sigB & ~2) : sigB) + (subC ? (-sigC & ~2) : sigC);
#ifdef FPU_DEBUG
    std::cout << "H_add : " << Float32<4>(signA,expZ,sigA) << '\n';
    std::cout << "M_add : " << Float32<4>(signB,expZ,(subB ? (-sigB & ~2) : sigB)) << " (inv: " << (!!subB) << ")\n";
    std::cout << "L_add : " << Float32<4>(signC,expZ,(subC ? (-sigC & ~2) : sigC)) << " (inv: " << (!!subC) << ")\n\n";
    std::cout << "z_sum : " << Float32<4>(signZ,expZ,sigZ) << "\n\n";
#endif
    if ( sigZ >= 0x80000000 ) {
        signZ = !signZ;
        sigZ = (-sigZ & ~2);
#ifdef FPU_DEBUG
        std::cout << "z_neg : " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( sigZ >= 0x10000000 ) {
        expZ += 1;
        sigZ = ((sigZ >> 1) & ~2) | ((sigZ >> 2) & 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj1: " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( sigZ >= 0x10000000 ) {
        expZ += 1;
        sigZ = ((sigZ >> 1) & ~2) | ((sigZ >> 2) & 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj2: " << Float32<4>(signZ,expZ,sigZ) << '\n';
#endif
    }
    if ( !sigZ ) {
        if ( subB || subC ) {
            uiZ =
                packToF32UI(
                    (softfloat_roundingMode == softfloat_round_min), 0, 0 );
            goto uiZ;
        } else if ( !expDiffC ) {
            uiZ = packToF32UI(signZ, expZ, 0);
            goto uiZ;
        }
    }
    sigZ = ((sigZ & ~1) << 2) | (sigZ & 1);
    if ( sigZ < 0x40000000 ) {
        --expZ;
        sigZ = ((sigZ & ~1) << 1) | (sigZ & 1);
#ifdef FPU_DEBUG
        std::cout << "z_adj3: " << Float32<7>(signZ,expZ,sigZ) << '\n';
#endif
    }
    shiftDist = softfloat_countLeadingZeros32( sigZ ) - 1;
    expZ -= shiftDist;
    sigZ = ((sigZ & ~1) << shiftDist) | (sigZ & 1);
#ifdef FPU_DEBUG
    std::cout << "z_adj4: " << Float32<7>(signZ,expZ,sigZ) << " (" << int(shiftDist) << ")\n";
    {
        float32_t fZ;
        fZ = softfloat_roundPackToF32( signZ, expZ, sigZ );
        std::cout << "z_rslt: " << fZ << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
    }
#endif
    return softfloat_roundPackToF32( signZ, expZ, sigZ );
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 generateNaN:
    softfloat_raiseFlags( softfloat_flag_invalid );
    uiZ = defaultNaNF32UI;
    goto uiZ;
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
 propagateNaN:
    uiZ = softfloat_propagateNaNF32UI( uiA, uiB, uiC );
 uiZ:
    uZ.ui = uiZ;
#ifdef FPU_DEBUG
    std::cout << "z_rslt: " << uZ.f << " [flags: " << SOFTFLOAT_FLAGS << "]\n";
#endif
    return uZ.f;
}


float32_t f1632_mulAdd2(
    float16_t a1, float16_t b1, float16_t a2, float16_t b2 )
{
    union ui16_f16 uA1;
    uint_fast16_t uiA1;
    union ui16_f16 uA2;
    uint_fast16_t uiA2;
    union ui16_f16 uB1;
    uint_fast16_t uiB1;
    union ui16_f16 uB2;
    uint_fast16_t uiB2;
    uint_fast32_t uiP1;
    uint_fast32_t uiP2;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA1.f = a1;
    uiA1 = uA1.ui;
    uA2.f = a2;
    uiA2 = uA2.ui;
    uB1.f = b1;
    uiB1 = uB1.ui;
    uB2.f = b2;
    uiB2 = uB2.ui;
#ifdef SOFTFLOAT_DENORMALS_TO_ZERO
    if ( isSubnormalF16UI( uiA1 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiA1 = softfloat_zeroExpSigF16UI( uiA1 );
    }
    if ( isSubnormalF16UI( uiB1 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiB1 = softfloat_zeroExpSigF16UI( uiB1 );
    }
    if ( isSubnormalF16UI( uiA2 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiA2 = softfloat_zeroExpSigF16UI( uiA2 );
    }
    if ( isSubnormalF16UI( uiB2 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiB2 = softfloat_zeroExpSigF16UI( uiB2 );
    }
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
#ifdef FPU_DEBUG
    std::cout << "\n----- p1 = a1 * b1 --------------------------------------------------------\n";
#endif
    uiP1 = f16_mulExt( uiA1, uiB1 );
#ifdef FPU_DEBUG
    std::cout << "\n----- p2 = a2 * b2 --------------------------------------------------------\n";
#endif
    uiP2 = f16_mulExt( uiA2, uiB2 );
#ifdef FPU_DEBUG
    std::cout << "\n----- z = p1 + p2 ---------------------------------------------------------\n";
#endif
    return f32_add2( uiP1, uiP2 );
}


float32_t f1632_mulAdd3(
    float16_t a1, float16_t b1, float16_t a2, float16_t b2, float32_t c )
{
    union ui16_f16 uA1;
    uint_fast16_t uiA1;
    union ui16_f16 uA2;
    uint_fast16_t uiA2;
    union ui16_f16 uB1;
    uint_fast16_t uiB1;
    union ui16_f16 uB2;
    uint_fast16_t uiB2;
    union ui32_f32 uC;
    uint_fast32_t uiC;
    uint_fast32_t uiP1;
    uint_fast32_t uiP2;

    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
    uA1.f = a1;
    uiA1 = uA1.ui;
    uA2.f = a2;
    uiA2 = uA2.ui;
    uB1.f = b1;
    uiB1 = uB1.ui;
    uB2.f = b2;
    uiB2 = uB2.ui;
    uC.f = c;
    uiC = uC.ui;
#ifdef SOFTFLOAT_DENORMALS_TO_ZERO
    if ( isSubnormalF16UI( uiA1 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiA1 = softfloat_zeroExpSigF16UI( uiA1 );
    }
    if ( isSubnormalF16UI( uiB1 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiB1 = softfloat_zeroExpSigF16UI( uiB1 );
    }
    if ( isSubnormalF16UI( uiA2 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiA2 = softfloat_zeroExpSigF16UI( uiA2 );
    }
    if ( isSubnormalF16UI( uiB2 ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiB2 = softfloat_zeroExpSigF16UI( uiB2 );
    }
    if ( isSubnormalF32UI( uiC ) ) {
        softfloat_raiseFlags( softfloat_flag_denormal );
        uiC = softfloat_zeroExpSigF32UI( uiC );
    }
#endif
    /*------------------------------------------------------------------------
    *------------------------------------------------------------------------*/
#ifdef FPU_DEBUG
    std::cout << "\n----- p1 = a1 * b1 --------------------------------------------------------\n";
#endif
    uiP1 = f16_mulExt( uiA1, uiB1 );
#ifdef FPU_DEBUG
    std::cout << "\n----- p2 = a2 * b2 --------------------------------------------------------\n";
#endif
    uiP2 = f16_mulExt( uiA2, uiB2 );
#ifdef FPU_DEBUG
    std::cout << "\n----- z = p1 + p2 + c -----------------------------------------------------\n";
#endif
    return f32_add3( uiP1, uiP2, uiC );
}
