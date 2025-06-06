/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        arm_fir_init_f64.c
 * Description:  Floating-point FIR filter initialization function
 *
 * $Date:        13 September 2021
 * $Revision:    V1.10.0
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
  @addtogroup FIR
  @{
 */

/**
  @brief         Initialization function for the floating-point FIR filter.
  @param[in,out] S          points to an instance of the floating-point FIR filter structure
  @param[in]     numTaps    number of filter coefficients in the filter
  @param[in]     pCoeffs    points to the filter coefficients buffer
  @param[in]     pState     points to the state buffer
  @param[in]     blockSize  number of samples processed per call

  @par           Details
                   <code>pCoeffs</code> points to the array of filter coefficients stored in time reversed order:
  <pre>
      {b[numTaps-1], b[numTaps-2], b[N-2], ..., b[1], b[0]}
  </pre>
  @par
                   <code>pState</code> points to the array of state variables.
                   <code>pState</code> is of length <code>numTaps+blockSize-1</code> samples, where <code>blockSize</code> is the number of input samples processed by each call to <code>arm_fir_f64()</code>.
  
  @par
                   There is no Helium version of the fir F64.

 */

ARM_DSP_ATTRIBUTE void arm_fir_init_f64(
    arm_fir_instance_f64 * S,
    uint16_t numTaps,
    const float64_t * pCoeffs,
    float64_t * pState,
    uint32_t blockSize)
{
    /* Assign filter taps */
    S->numTaps = numTaps;
    
    /* Assign coefficient pointer */
    S->pCoeffs = pCoeffs;
    
    /* Clear state buffer. The size is always (blockSize + numTaps - 1) */
    memset(pState, 0, (numTaps + (blockSize - 1U)) * sizeof(float64_t));
    /* Assign state pointer */
    S->pState = pState;
}

/**
  @} end of FIR group
 */
