/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_lms_norm_q31.c
 * Description:  Processing function for the Q31 NLMS filter
 *
 * $Date:        23 April 2021
 * $Revision:    V1.9.0
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- */
/*
 * Copyright (C) 2010-2021 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "dsp/filtering_functions.h"

/**
  @ingroup groupFilters
 */

/**
  @addtogroup LMS_NORM
  @{
 */

/**
  @brief         Processing function for Q31 normalized LMS filter.
  @param[in]     S         points to an instance of the Q31 normalized LMS filter structure
  @param[in]     pSrc      points to the block of input data
  @param[in]     pRef      points to the block of reference data
  @param[out]    pOut      points to the block of output data
  @param[out]    pErr      points to the block of error data
  @param[in]     blockSize number of samples to process

  @par           Scaling and Overflow Behavior
                   The function is implemented using an internal 64-bit accumulator.
                   The accumulator has a 2.62 format and maintains full precision of the intermediate
                   multiplication results but provides only a single guard bit.
                   Thus, if the accumulator result overflows it wraps around rather than clip.
                   In order to avoid overflows completely the input signal must be scaled down by
                   log2(numTaps) bits. The reference signal should not be scaled down.
                   After all multiply-accumulates are performed, the 2.62 accumulator is shifted
                   and saturated to 1.31 format to yield the final result.
                   The output signal and error signal are in 1.31 format.
 @par
  	               In this filter, filter coefficients are updated for each sample and the
                   updation of filter cofficients are saturted.
 */

ARM_DSP_ATTRIBUTE void arm_lms_norm_q31(
        arm_lms_norm_instance_q31 * S,
  const q31_t * pSrc,
        q31_t * pRef,
        q31_t * pOut,
        q31_t * pErr,
        uint32_t blockSize)
{
        q31_t *pState = S->pState;                     /* State pointer */
        q31_t *pCoeffs = S->pCoeffs;                   /* Coefficient pointer */
        q31_t *pStateCurnt;                            /* Points to the current sample of the state */
        q31_t *px, *pb;                                /* Temporary pointers for state and coefficient buffers */
        q31_t mu = S->mu;                              /* Adaptive factor */
        uint32_t numTaps = S->numTaps;                 /* Number of filter coefficients in the filter */
        uint32_t tapCnt, blkCnt;                       /* Loop counters */
        q63_t acc;                                     /* Accumulator */
        q63_t energy;                                  /* Energy of the input */
        q31_t e = 0;                                   /* Error data sample */
        q31_t w = 0, in;                               /* Weight factor and state */
        q31_t x0;                                      /* Temporary variable to hold input sample */
        q31_t errorXmu, oneByEnergy;                   /* Temporary variables to store error and mu product and reciprocal of energy */
        q31_t postShift;                               /* Post shift to be applied to weight after reciprocal calculation */
        q31_t coef;                                    /* Temporary variable for coef */
        q31_t acc_l, acc_h;                            /* Temporary input */
        uint32_t uShift = ((uint32_t) S->postShift + 1U);
        uint32_t lShift = 32U - uShift;                /*  Shift to be applied to the output */

  energy = S->energy;
  x0 = S->x0;

  /* S->pState points to buffer which contains previous frame (numTaps - 1) samples */
  /* pStateCurnt points to the location where the new input data should be written */
  pStateCurnt = &(S->pState[(numTaps - 1U)]);

  /* initialise loop count */
  blkCnt = blockSize;

  while (blkCnt > 0U)
  {
    /* Copy the new input sample into the state buffer */
    *pStateCurnt++ = *pSrc;

    /* Initialize pState pointer */
    px = pState;

    /* Initialize coefficient pointer */
    pb = pCoeffs;

    /* Read the sample from input buffer */
    in = *pSrc++;

    /* Update the energy calculation */
    energy = (q31_t) ((((q63_t) energy << 32) - (((q63_t) x0 * x0) << 1)) >> 32);
    energy = ((((q63_t) in * in) << 1) + (energy << 32)) >> 32;
    energy = clip_q63_to_q31(energy);
    
    /* Set the accumulator to zero */
    acc = 0;

#if defined (ARM_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time. */
    tapCnt = numTaps >> 2U;

    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */
      /* acc +=  b[N] * x[n-N] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-1] * x[n-N-1] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-2] * x[n-N-2] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* acc +=  b[N-3] * x[n-N-3] */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps % 0x4U;

#else

    /* Initialize tapCnt with number of samples */
    tapCnt = numTaps;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */
      acc += ((q63_t) (*px++)) * (*pb++);

      /* Decrement the loop counter */
      tapCnt--;
    }

    /* Converting the result to 1.31 format */
    /* Calc lower part of acc */
    acc_l = acc & 0xffffffff;

    /* Calc upper part of acc */
    acc_h = (acc >> 32) & 0xffffffff;

    acc = (uint32_t) acc_l >> lShift | acc_h << uShift;

    /* Store the result from accumulator into the destination buffer. */
    *pOut++ = (q31_t) acc;

    /* Compute and store error */
    e = *pRef++ - (q31_t) acc;
    *pErr++ = e;

    /* Calculates the reciprocal of energy */
    postShift = arm_recip_q31(energy + DELTA_Q31, &oneByEnergy, &S->recipTable[0]);

    /* Calculation of product of (e * mu) */
    errorXmu = (q31_t) (((q63_t) e * mu) >> 31);

    /* Weighting factor for the normalized version */
    w = clip_q63_to_q31(((q63_t) errorXmu * oneByEnergy) >> (31 - postShift));

    /* Initialize pState pointer */
    px = pState;

    /* Initialize coefficient pointer */
    pb = pCoeffs;

#if defined (ARM_MATH_LOOPUNROLL)

    /* Loop unrolling: Compute 4 taps at a time. */
    tapCnt = numTaps >> 2U;

    /* Update filter coefficients */
    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */

      /* coef is in 2.30 format */
      coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
      /* get coef in 1.31 format by left shifting */
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      /* update coefficient buffer to next coefficient */
      pb++;

      coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Loop unrolling: Compute remaining taps */
    tapCnt = numTaps % 0x4U;

#else

    /* Initialize tapCnt with number of samples */
    tapCnt = numTaps;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

    while (tapCnt > 0U)
    {
      /* Perform the multiply-accumulate */
      coef = (q31_t) (((q63_t) w * (*px++)) >> (32));
      *pb = clip_q63_to_q31((q63_t) * pb + (coef << 1U));
      pb++;

      /* Decrement loop counter */
      tapCnt--;
    }

    /* Read the sample from state buffer */
    x0 = *pState;

    /* Advance state pointer by 1 for the next sample */
    pState = pState + 1;

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Save energy and x0 values for the next frame */
  S->energy = (q31_t) energy;
  S->x0 = x0;

  /* Processing is complete.
     Now copy the last numTaps - 1 samples to the start of the state buffer.
     This prepares the state buffer for the next function call. */

  /* Points to the start of the pState buffer */
  pStateCurnt = S->pState;

  /* copy data */
#if defined (ARM_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 taps at a time. */
  tapCnt = (numTaps - 1U) >> 2U;

  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

  /* Loop unrolling: Compute remaining taps */
  tapCnt = (numTaps - 1U) % 0x4U;

#else

  /* Initialize tapCnt with number of samples */
  tapCnt = (numTaps - 1U);

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */

  while (tapCnt > 0U)
  {
    *pStateCurnt++ = *pState++;

    /* Decrement loop counter */
    tapCnt--;
  }

}

/**
  @} end of LMS_NORM group
 */
