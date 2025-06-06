/*
 * Copyright (c) 2013-2023 Arm Limited. All rights reserved.
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
 *
 * -----------------------------------------------------------------------------
 *
 * Project:     CMSIS-RTOS RTX
 * Title:       Cortex-M Core definitions
 *
 * -----------------------------------------------------------------------------
 */

#ifndef RTX_CORE_CM_H_
#define RTX_CORE_CM_H_

#ifndef RTX_CORE_C_H_
#ifndef RTE_COMPONENTS_H
#include "RTE_Components.h"
#endif
#include CMSIS_device_header
#endif

#include <stdbool.h>
typedef bool bool_t;

#ifndef FALSE
#define FALSE                   ((bool_t)0)
#endif

#ifndef TRUE
#define TRUE                    ((bool_t)1)
#endif

#ifndef EXCLUSIVE_ACCESS
#if   ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) || \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)) || \
       (defined(__ARM_ARCH_8M_BASE__)   && (__ARM_ARCH_8M_BASE__   != 0)) || \
       (defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) || \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)))
#define EXCLUSIVE_ACCESS        1
#else
#define EXCLUSIVE_ACCESS        0
#endif
#endif

#define OS_TICK_HANDLER         SysTick_Handler

/// xPSR_Initialization Value
/// \param[in]  privileged      true=privileged, false=unprivileged
/// \param[in]  thumb           true=Thumb, false=ARM
/// \return                     xPSR Init Value
__STATIC_INLINE uint32_t xPSR_InitVal (bool_t privileged, bool_t thumb) {
  (void)privileged;
  (void)thumb;
  return (0x01000000U);
}

// Stack Frame:
//  - Extended: S16-S31, R4-R11, R0-R3, R12, LR, PC, xPSR, S0-S15, FPSCR
//  - Basic:             R4-R11, R0-R3, R12, LR, PC, xPSR

/// Stack Frame Initialization Value (EXC_RETURN[7..0])
#if (DOMAIN_NS == 1)
#define STACK_FRAME_INIT_VAL    0xBCU
#else
#define STACK_FRAME_INIT_VAL    0xFDU
#endif

/// Stack Offset of Register R0
/// \param[in]  stack_frame     Stack Frame (EXC_RETURN[7..0])
/// \return                     R0 Offset
__STATIC_INLINE uint32_t StackOffsetR0 (uint8_t stack_frame) {
#if ((__FPU_USED == 1U) || \
     (defined(__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE > 0)))
  return (((stack_frame & 0x10U) == 0U) ? ((16U+8U)*4U) : (8U*4U));
#else
  (void)stack_frame;
  return (8U*4U);
#endif
}


//  ==== Core functions ====

//lint -sem(__get_CONTROL, pure)
//lint -sem(__get_IPSR,    pure)
//lint -sem(__get_PRIMASK, pure)
//lint -sem(__get_BASEPRI, pure)

/// Check if running Privileged
/// \return     true=privileged, false=unprivileged
__STATIC_INLINE bool_t IsPrivileged (void) {
  return ((__get_CONTROL() & 1U) == 0U);
}

/// Set thread Privileged mode
/// \param[in]  privileged      true=privileged, false=unprivileged
__STATIC_INLINE void SetPrivileged (bool_t privileged) {
  if (privileged) {
    // Privileged Thread mode & PSP
    __set_CONTROL(0x02U);
  } else {
    // Unprivileged Thread mode & PSP
    __set_CONTROL(0x03U);
  }
}

/// Check if in Exception
/// \return     true=exception, false=thread
__STATIC_INLINE bool_t IsException (void) {
  return (__get_IPSR() != 0U);
}

/// Check if in Fault
/// \return     true, false
__STATIC_INLINE bool_t IsFault (void) {
  uint32_t ipsr = __get_IPSR();
  return (((int32_t)ipsr < ((int32_t)SVCall_IRQn + 16)) &&
          ((int32_t)ipsr > ((int32_t)NonMaskableInt_IRQn + 16)));
}

/// Check if in SVCall IRQ
/// \return     true, false
__STATIC_INLINE bool_t IsSVCallIrq (void) {
  return ((int32_t)__get_IPSR() == ((int32_t)SVCall_IRQn + 16));
}

/// Check if in PendSV IRQ
/// \return     true, false
__STATIC_INLINE bool_t IsPendSvIrq (void) {
  return ((int32_t)__get_IPSR() == ((int32_t)PendSV_IRQn + 16));
}

/// Check if in Tick Timer IRQ
/// \return     true, false
__STATIC_INLINE bool_t IsTickIrq (int32_t tick_irqn) {
  return ((int32_t)__get_IPSR() == (tick_irqn + 16));
}

/// Check if IRQ is Masked
/// \return     true=masked, false=not masked
__STATIC_INLINE bool_t IsIrqMasked (void) {
#if   ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) || \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)) || \
       (defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) || \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)))
  return ((__get_PRIMASK() != 0U) || (__get_BASEPRI() != 0U));
#else
  return  (__get_PRIMASK() != 0U);
#endif
}


//  ==== Core Peripherals functions ====

/// Setup SVC and PendSV System Service Calls
__STATIC_INLINE void SVC_Setup (void) {
#if   ((defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) || \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)) || \
       (defined(__CORTEX_M)             && (__CORTEX_M == 7U)))
  uint32_t p, n;

  SCB->SHPR[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHPR[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }
  SCB->SHPR[7] = (uint8_t)(0xFEU << n);
#elif  (defined(__ARM_ARCH_8M_BASE__)   && (__ARM_ARCH_8M_BASE__   != 0))
  uint32_t n;

  SCB->SHPR[1] |= 0x00FF0000U;
  n = SCB->SHPR[1];
  SCB->SHPR[0] |= (n << (8+1)) & 0xFC000000U;
#elif ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) || \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)))
  uint32_t p, n;

  SCB->SHPR[10] = 0xFFU;
  n = 32U - (uint32_t)__CLZ(~(SCB->SHPR[10] | 0xFFFFFF00U));
  p = NVIC_GetPriorityGrouping();
  if (p >= n) {
    n = p + 1U;
  }
  SCB->SHPR[7] = (uint8_t)(0xFEU << n);
#elif  (defined(__ARM_ARCH_6M__)        && (__ARM_ARCH_6M__        != 0))
  uint32_t n;

  SCB->SHPR[1] |= 0x00FF0000U;
  n = SCB->SHPR[1];
  SCB->SHPR[0] |= (n << (8+1)) & 0xFC000000U;
#endif
}

/// Get Pending SV (Service Call) Flag
/// \return     Pending SV Flag
__STATIC_INLINE uint8_t GetPendSV (void) {
  return ((uint8_t)((SCB->ICSR & (SCB_ICSR_PENDSVSET_Msk)) >> 24));
}

/// Clear Pending SV (Service Call) Flag
__STATIC_INLINE void ClrPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;
}

/// Set Pending SV (Service Call) Flag
__STATIC_INLINE void SetPendSV (void) {
  SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}


//  ==== Service Calls definitions ====

//lint -save -e9023 -e9024 -e9026 "Function-like macros using '#/##'" [MISRA Note 10]

#if defined(__ICCARM__)

#if defined(RTX_SVC_PTR_CHECK)
#warning "SVC Function Pointer checking is not supported!"
#endif

#if   ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) ||   \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)) ||   \
       (defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) ||   \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)))
#define SVC_ArgF(f)                                                            \
  __asm(                                                                       \
    "mov r12,%0\n"                                                             \
    :: "r"(&f): "r12"                                                          \
  );
#elif ((defined(__ARM_ARCH_6M__)        && (__ARM_ARCH_6M__        != 0)) ||   \
       (defined(__ARM_ARCH_8M_BASE__)   && (__ARM_ARCH_8M_BASE__   != 0)))
#define SVC_ArgF(f)                                                            \
  __asm(                                                                       \
    "mov r7,%0\n"                                                              \
    :: "r"(&f): "r7"                                                           \
  );
#endif

#define STRINGIFY(a) #a
#define SVC_INDIRECT(n) _Pragma(STRINGIFY(svc_number = n)) __svc

#define SVC0_0N(f,t)                                                           \
SVC_INDIRECT(0) t    svc##f ();                                                \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (void) {                                           \
  SVC_ArgF(svcRtx##f);                                                         \
  svc##f();                                                                    \
}

#define SVC0_0(f,t)                                                            \
SVC_INDIRECT(0) t    svc##f ();                                                \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (void) {                                           \
  SVC_ArgF(svcRtx##f);                                                         \
  return svc##f();                                                             \
}

#define SVC0_1N(f,t,t1)                                                        \
SVC_INDIRECT(0) t    svc##f (t1 a1);                                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (t1 a1) {                                          \
  SVC_ArgF(svcRtx##f);                                                         \
  svc##f(a1);                                                                  \
}

#define SVC0_1(f,t,t1)                                                         \
SVC_INDIRECT(0) t    svc##f (t1 a1);                                           \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (t1 a1) {                                          \
  SVC_ArgF(svcRtx##f);                                                         \
  return svc##f(a1);                                                           \
}

#define SVC0_2(f,t,t1,t2)                                                      \
SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2);                                    \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (t1 a1, t2 a2) {                                   \
  SVC_ArgF(svcRtx##f);                                                         \
  return svc##f(a1,a2);                                                        \
}

#define SVC0_3(f,t,t1,t2,t3)                                                   \
SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3);                             \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (t1 a1, t2 a2, t3 a3) {                            \
  SVC_ArgF(svcRtx##f);                                                         \
  return svc##f(a1,a2,a3);                                                     \
}

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
SVC_INDIRECT(0) t    svc##f (t1 a1, t2 a2, t3 a3, t4 a4);                      \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t  __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                     \
  SVC_ArgF(svcRtx##f);                                                         \
  return svc##f(a1,a2,a3,a4);                                                  \
}

#else   // !defined(__ICCARM__)

//lint -esym(522,__svc*) "Functions '__svc*' are impure (side-effects)"

#if   ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) ||   \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)) ||   \
       (defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) ||   \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)))
#define SVC_RegF "r12"
#elif ((defined(__ARM_ARCH_6M__)        && (__ARM_ARCH_6M__        != 0)) ||   \
       (defined(__ARM_ARCH_8M_BASE__)   && (__ARM_ARCH_8M_BASE__   != 0)))
#define SVC_RegF "r7"
#endif

#define SVC_ArgN(n) \
register uint32_t __r##n __ASM("r"#n)

#define SVC_ArgR(n,a) \
register uint32_t __r##n __ASM("r"#n) = (uint32_t)a

#if    (defined(RTX_SVC_PTR_CHECK) && !defined(_lint))
#define SVC_ArgF(f) \
register uint32_t __rf   __ASM(SVC_RegF) = (uint32_t)jmpRtx##f
#else
#define SVC_ArgF(f) \
register uint32_t __rf   __ASM(SVC_RegF) = (uint32_t)svcRtx##f
#endif

#define SVC_In0 "r"(__rf)
#define SVC_In1 "r"(__rf),"r"(__r0)
#define SVC_In2 "r"(__rf),"r"(__r0),"r"(__r1)
#define SVC_In3 "r"(__rf),"r"(__r0),"r"(__r1),"r"(__r2)
#define SVC_In4 "r"(__rf),"r"(__r0),"r"(__r1),"r"(__r2),"r"(__r3)

#define SVC_Out0
#define SVC_Out1 "=r"(__r0)

#define SVC_CL0
#define SVC_CL1 "r0"

#define SVC_Call0(in, out, cl)                                                 \
  __ASM volatile ("svc 0" : out : in : cl)

#if    (defined(RTX_SVC_PTR_CHECK) && !defined(_lint))
#if   ((defined(__ARM_ARCH_7M__)        && (__ARM_ARCH_7M__        != 0)) ||   \
       (defined(__ARM_ARCH_7EM__)       && (__ARM_ARCH_7EM__       != 0)) ||   \
       (defined(__ARM_ARCH_8M_MAIN__)   && (__ARM_ARCH_8M_MAIN__   != 0)) ||   \
       (defined(__ARM_ARCH_8_1M_MAIN__) && (__ARM_ARCH_8_1M_MAIN__ != 0)))
#define SVC_Jump(f)                                                            \
  __ASM volatile (                                                             \
    ".align 2\n\t"                                                             \
    "b.w %[adr]" : : [adr] "X" (f)                                             \
  )
#elif ((defined(__ARM_ARCH_6M__)        && (__ARM_ARCH_6M__        != 0)) ||   \
       (defined(__ARM_ARCH_8M_BASE__)   && (__ARM_ARCH_8M_BASE__   != 0)))
#define SVC_Jump(f)                                                            \
  __ASM volatile (                                                             \
    ".align 3\n\t"                                                             \
    "ldr r7,1f\n\t"                                                            \
    "bx  r7\n"                                                                 \
    "1: .word %[adr]" : : [adr] "X" (f)                                        \
  )
#endif
#define SVC_Veneer_Prototye(f)                                                 \
__STATIC_INLINE void jmpRtx##f (void);
#define SVC_Veneer_Function(f)                                                 \
__attribute__((naked,section(".text.os.svc.veneer."#f)))                       \
__STATIC_INLINE void jmpRtx##f (void) {                                        \
  SVC_Jump(svcRtx##f);                                                         \
}
#else
#define SVC_Veneer_Prototye(f)
#define SVC_Veneer_Function(f)
#endif

#define SVC0_0N(f,t)                                                           \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In0, SVC_Out0, SVC_CL1);                                       \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_0(f,t)                                                            \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (void) {                                            \
  SVC_ArgN(0);                                                                 \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In0, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_1N(f,t,t1)                                                        \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1) {                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In1, SVC_Out1, SVC_CL0);                                       \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_1(f,t,t1)                                                         \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1) {                                           \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In1, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_2(f,t,t1,t2)                                                      \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2) {                                    \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In2, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_3(f,t,t1,t2,t3)                                                   \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3) {                             \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In3, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}                                                                              \
SVC_Veneer_Function(f)

#define SVC0_4(f,t,t1,t2,t3,t4)                                                \
SVC_Veneer_Prototye(f)                                                         \
__attribute__((always_inline))                                                 \
__STATIC_INLINE t __svc##f (t1 a1, t2 a2, t3 a3, t4 a4) {                      \
  SVC_ArgR(0,a1);                                                              \
  SVC_ArgR(1,a2);                                                              \
  SVC_ArgR(2,a3);                                                              \
  SVC_ArgR(3,a4);                                                              \
  SVC_ArgF(f);                                                                 \
  SVC_Call0(SVC_In4, SVC_Out1, SVC_CL0);                                       \
  return (t) __r0;                                                             \
}                                                                              \
SVC_Veneer_Function(f)

#endif

//lint -restore [MISRA Note 10]


//  ==== Exclusive Access Operation ====

#if (EXCLUSIVE_ACCESS == 1)

//lint ++flb "Library Begin" [MISRA Note 12]

/// Atomic Access Operation: Write (8-bit)
/// \param[in]  mem             Memory address
/// \param[in]  val             Value to write
/// \return                     Previous value
__STATIC_INLINE uint8_t atomic_wr8 (uint8_t *mem, uint8_t val) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint8_t  ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexb %[ret],[%[mem]]\n\t"
    "strexb %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],2f\n\t"
    "b       1b\n"
  "2:"
  : [ret] "=&l" (ret),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [val] "l"   (val)
  : "memory"
  );

  return ret;
}

/// Atomic Access Operation: Set bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     New value
__STATIC_INLINE uint32_t atomic_set32 (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[val],[%[mem]]\n\t"
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    "mov   %[ret],%[val]\n\t"
    "orrs  %[ret],%[bits]\n\t"
#else
    "orr   %[ret],%[val],%[bits]\n\t"
#endif
    "strex %[res],%[ret],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
  : "memory", "cc"
#else
  : "memory"
#endif
  );

  return ret;
}

/// Atomic Access Operation: Clear bits (32-bit)
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Previous value
__STATIC_INLINE uint32_t atomic_clr32 (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
  : "memory", "cc"
#else
  : "memory"
#endif
  );

  return ret;
}

/// Atomic Access Operation: Check if all specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
__STATIC_INLINE uint32_t atomic_chk32_all (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    "mov   %[val],%[ret]\n\t"
    "ands  %[val],%[bits]\n\t"
#else
    "and   %[val],%[ret],%[bits]\n\t"
#endif
    "cmp   %[val],%[bits]\n\t"
    "beq   2f\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     3f\n"
  "2:\n\t"
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Check if any specified bits (32-bit) are active and clear them
/// \param[in]  mem             Memory address
/// \param[in]  bits            Bit mask
/// \return                     Active bits before clearing or 0 if not active
__STATIC_INLINE uint32_t atomic_chk32_any (uint32_t *mem, uint32_t bits) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "tst   %[ret],%[bits]\n\t"
    "bne   2f\n\t"
    "clrex\n\t"
    "movs  %[ret],#0\n\t"
    "b     3f\n"
  "2:\n\t"
#if (defined(__ARM_ARCH_8M_BASE__) && (__ARM_ARCH_8M_BASE__ != 0))
    "mov   %[val],%[ret]\n\t"
    "bics  %[val],%[bits]\n\t"
#else
    "bic   %[val],%[ret],%[bits]\n\t"
#endif
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [mem]  "l"   (mem),
    [bits] "l"   (bits)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Increment (32-bit)
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint32_t atomic_inc32 (uint32_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "adds  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Increment (16-bit) if Less Than
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
__STATIC_INLINE uint16_t atomic_inc16_lt (uint16_t *mem, uint16_t max) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cmp    %[max],%[ret]\n\t"
    "bhi    2f\n\t"
    "clrex\n\t"
    "b      3f\n"
  "2:\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [max] "l"   (max)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Increment (16-bit) and clear on Limit
/// \param[in]  mem             Memory address
/// \param[in]  max             Maximum value
/// \return                     Previous value
__STATIC_INLINE uint16_t atomic_inc16_lim (uint16_t *mem, uint16_t lim) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "adds   %[val],%[ret],#1\n\t"
    "cmp    %[lim],%[val]\n\t"
    "bhi    2f\n\t"
    "movs   %[val],#0\n"
  "2:\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem),
    [lim] "l"   (lim)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Decrement (32-bit)
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint32_t atomic_dec32 (uint32_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "subs  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Decrement (32-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint32_t atomic_dec32_nz (uint32_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint32_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[mem]]\n\t"
    "cbnz  %[ret],2f\n\t"
    "clrex\n\t"
    "b     3f\n"
  "2:\n\t"
    "subs  %[val],%[ret],#1\n\t"
    "strex %[res],%[val],[%[mem]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Decrement (16-bit) if Not Zero
/// \param[in]  mem             Memory address
/// \return                     Previous value
__STATIC_INLINE uint16_t atomic_dec16_nz (uint16_t *mem) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register uint16_t ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrexh %[ret],[%[mem]]\n\t"
    "cbnz   %[ret],2f\n\t"
    "clrex\n\t"
    "b      3f\n"
  "2:\n\t"
    "subs   %[val],%[ret],#1\n\t"
    "strexh %[res],%[val],[%[mem]]\n\t"
    "cbz    %[res],3f\n\t"
    "b      1b\n"
  "3:"
  : [ret] "=&l" (ret),
    [val] "=&l" (val),
    [res] "=&l" (res)
  : [mem] "l"   (mem)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Link Get
/// \param[in]  root            Root address
/// \return                     Link
__STATIC_INLINE void *atomic_link_get (void **root) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif
  register void    *ret;

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldrex %[ret],[%[root]]\n\t"
    "cbnz  %[ret],2f\n\t"
    "clrex\n\t"
    "b     3f\n"
  "2:\n\t"
    "ldr   %[val],[%[ret]]\n\t"
    "strex %[res],%[val],[%[root]]\n\t"
    "cbz   %[res],3f\n\t"
    "b     1b\n"
  "3:"
  : [ret]  "=&l" (ret),
    [val]  "=&l" (val),
    [res]  "=&l" (res)
  : [root] "l"   (root)
  : "cc", "memory"
  );

  return ret;
}

/// Atomic Access Operation: Link Put
/// \param[in]  root            Root address
/// \param[in]  lnk             Link
__STATIC_INLINE void atomic_link_put (void **root, void *link) {
#ifdef  __ICCARM__
#pragma diag_suppress=Pe550
#endif
  register uint32_t val1, val2, res;
#ifdef  __ICCARM__
#pragma diag_default=Pe550
#endif

  __ASM volatile (
#ifndef __ICCARM__
  ".syntax unified\n\t"
#endif
  "1:\n\t"
    "ldr   %[val1],[%[root]]\n\t"
    "str   %[val1],[%[link]]\n\t"
    "dmb\n\t"
    "ldrex %[val1],[%[root]]\n\t"
    "ldr   %[val2],[%[link]]\n\t"
    "cmp   %[val2],%[val1]\n\t"
    "bne   1b\n\t"
    "strex %[res],%[link],[%[root]]\n\t"
    "cbz   %[res],2f\n\t"
    "b     1b\n"
  "2:"
  : [val1] "=&l" (val1),
    [val2] "=&l" (val2),
    [res]  "=&l" (res)
  : [root] "l"   (root),
    [link] "l"   (link)
  : "cc", "memory"
  );
}

//lint --flb "Library End" [MISRA Note 12]

#endif  // (EXCLUSIVE_ACCESS == 1)


#endif  // RTX_CORE_CM_H_
