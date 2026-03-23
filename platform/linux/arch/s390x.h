#ifndef NT_S390_SYSTEM_CALL
#define NT_S390_SYSTEM_CALL 0x307
#endif

static struct ptrace_personality arch_personality[1] = {
    {
        offsetof(s390_regs, gprs[2]),
        offsetof(s390_regs, orig_gpr2),
        offsetof(s390_regs, gprs[3]),
        offsetof(s390_regs, gprs[4]),
        offsetof(s390_regs, gprs[5]),
        offsetof(s390_regs, gprs[6]),
        offsetof(s390_regs, gprs[7]),
        offsetof(s390_regs, psw.addr),
    }
};

static inline void arch_fixup_regs(struct ptrace_child *child) {
    child->regs.psw.addr -= S390_SYSCALL_SIZE;
}

static inline int arch_set_syscall(struct ptrace_child *child,
                                   unsigned long sysno) {
    unsigned int syscall_reg = sysno;
    struct iovec reg_iovec = {
        .iov_base = &syscall_reg,
        .iov_len = sizeof(syscall_reg)
    };
    return ptrace_command(child, PTRACE_SETREGSET, NT_S390_SYSTEM_CALL,
                          &reg_iovec);
}

static inline int arch_save_syscall(struct ptrace_child *child) {
    unsigned int syscall_reg;
    struct iovec reg_iovec = {
        .iov_base = &syscall_reg,
        .iov_len = sizeof(syscall_reg)
    };
    if (ptrace_command(child, PTRACE_GETREGSET, NT_S390_SYSTEM_CALL,
                       &reg_iovec) < 0)
        return -1;

    child->saved_syscall = syscall_reg;
    return 0;
}

static inline int arch_restore_syscall(struct ptrace_child *child) {
    return arch_set_syscall(child, child->saved_syscall);
}
