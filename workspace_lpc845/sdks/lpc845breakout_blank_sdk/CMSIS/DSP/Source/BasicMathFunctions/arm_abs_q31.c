/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_abs_q31.c
 * Description:  Q31 vector absolute value
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

#include "dsp/basic_math_functions.h"

/**
  @ingroup groupMath
 */

/**
  @addtogroup BasicAbs
  @{
 */

/**
  @brief         Q31 vector absolute value.
  @param[in]     pSrc       points to the input vector
  @param[out]    pDst       points to the output vector
  @param[in]     blockSize  number of samples in each vector

  @par           Scaling and Overflow Behavior
                   The function uses saturating arithmetic.
                   The Q31 value -1 (0x80000000) will be saturated to the maximum allowable positive value 0x7FFFFFFF.
 */

#if defined(ARM_MATH_MVEI) && !defined(ARM_MATH_AUTOVECTORIZE)

#include "arm_helium_utils.h"

ARM_DSP_ATTRIBUTE void arm_abs_q31(
    const q31_t * pSrc,
    q31_t * pDst,
    uint32_t blockSize)
{
    uint32_t  blkCnt;           /* Loop counters */
    q31x4_t vecSrc;

    /* Compute 4 outputs at a time */
    blkCnt = blockSize >> 2;

    while (blkCnt > 0U)
    {
        /*
         * C = |A|
         * Calculate absolute and then store the results in the destination buffer.
         */
        vecSrc = vld1q(pSrc);
        vst1q(pDst, vqabsq(vecSrc));
        /*
         * Decrement the blockSize loop counter
         */
        blkCnt--;
        /*
         * Advance vector source and destination pointers
         */
        pSrc += 4;
        pDst += 4;
    }
    /*
     * Tail
     */
    blkCnt = blockSize & 3;

    if (blkCnt > 0U)
    {
        mve_pred16_t p0 = vctp32q(blkCnt);
        vecSrc = vld1q(pSrc);
        vstrwq_p(pDst, vqabsq(vecSrc), p0);
    }
}

#else
ARM_DSP_ATTRIBUTE void arm_abs_q31(
  const q31_t * pSrc,
        q31_t * pDst,
        uint32_t blockSize)
{
        uint32_t blkCnt;                               /* Loop counter */
        q31_t in;                                      /* Temporary variable */

#if defined(ARM_MATH_NEON)
    int32x4_t vec1;
    int32x4_t res;

    /* Compute 4 outputs at a time */  
    blkCnt = blockSize >> 2U;

    while (blkCnt > 0U)
    {
        /* C = |A| */
        /* Calculate absolute and then store the results in the destination buffer. */

        vec1 = vld1q_s32(pSrc);
        res = vqabsq_s32(vec1);
        vst1q_s32(pDst, res);

        /* Increment pointers */
        pSrc += 4;
        pDst += 4;
        
        /* Decrement the blockSize loop counter */
        blkCnt--;
    }

    /* Tail */
    blkCnt = blockSize & 0x3;

#else
#if defined (ARM_MATH_LOOPUNROLL)

  /* Loop unrolling: Compute 4 outputs at a time */
  blkCnt = blockSize >> 2U;

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7fffffff) and store result in destination buffer. */
    in = *pSrc++;
#if defined (ARM_MATH_DSP)
    *pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (ARM_MATH_DSP)
    *pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (ARM_MATH_DSP)
    *pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    in = *pSrc++;
#if defined (ARM_MATH_DSP)
    *pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    /* Decrement loop counter */
    blkCnt--;
  }

  /* Loop unrolling: Compute remaining outputs */
  blkCnt = blockSize % 0x4U;

#else

  /* Initialize blkCnt with number of samples */
  blkCnt = blockSize;

#endif /* #if defined (ARM_MATH_LOOPUNROLL) */
#endif /* #if defined (ARM_MATH_NEON) */

  while (blkCnt > 0U)
  {
    /* C = |A| */

    /* Calculate absolute of input (if -1 then saturated to 0x7fffffff) and store result in destination buffer. */
    in = *pSrc++;
#if defined (ARM_MATH_DSP)
    *pDst++ = (in > 0) ? in : (q31_t)__QSUB(0, in);
#else
    *pDst++ = (in > 0) ? in : ((in == INT32_MIN) ? INT32_MAX : -in);
#endif

    /* Decrement loop counter */
    blkCnt--;
  }

}
#endif /* #if defined (ARM_MATH_MVEI) */
/**
  @} end of BasicAbs group
 */
