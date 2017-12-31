#pragma once

#include "process.h"

#include <stdbool.h>
#include <sys/user.h>


typedef unsigned long long int t_ureg;

/**
** \brief represents metadata about a register
*/
typedef struct ureg_meta
{
    // the register name
    const char *name;
    // its offset inside user_regs_struct
    size_t off;
} s_ureg_meta;


#define UREGS_APPLY(F)      \
    F(R15, r15)             \
    F(R14, r14)             \
    F(R13, r13)             \
    F(R12, r12)             \
    F(RBP, rbp)             \
    F(RBX, rbx)             \
    F(R11, r11)             \
    F(R10, r10)             \
    F(R9, r9)               \
    F(R8, r8)               \
    F(RAX, rax)             \
    F(RCX, rcx)             \
    F(RDX, rdx)             \
    F(RSI, rsi)             \
    F(RDI, rdi)             \
    F(ORIG_EAX, orig_rax)   \
    F(RIP, rip)             \
    F(CS, cs)               \
    F(EFLAGS, eflags)       \
    F(RSP, rsp)             \
    F(SS, ss)               \
    F(FS_BASE, fs_base)     \
    F(GS_BASE, gs_base)     \
    F(DS, ds)               \
    F(ES, es)               \
    F(FS, fs)               \
    F(GS, gs)


typedef enum uregs
{
#define ENUMIZE(EName, Field) UREG_ ## EName,
    UREGS_APPLY(ENUMIZE)
#undef ENUMIZE
    UREG_COUNT
} e_uregs;


extern s_ureg_meta g_ureg_meta[UREG_COUNT];


/**
** \brief retrives the value of a register
** \param proc the process to explore
** \param regid the register identifier
** \param res where to store the result
** \return whether the operation failed
*/
bool proc_getreg(s_proc *proc, e_uregs regid, t_ureg *res);


/**
** \brief sets the value of a register
** \param proc the process to explore
** \param regid the register identifier
** \param reg the register value
** \return whether the operation failed
*/
bool proc_setreg(s_proc *proc, e_uregs regid, t_ureg reg);


#define UREG_NAME(Regid) g_ureg_meta[Regid].name
#define UREG(Ur, Regid) (*(t_ureg*)((char*)(Ur) + g_ureg_meta[Regid].off))
