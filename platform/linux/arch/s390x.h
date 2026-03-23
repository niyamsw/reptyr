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
    child->saved_syscall = child->regs.gprs[2];
    child->regs.gprs[2] = child->regs.orig_gpr2;
}

static inline int arch_set_syscall(struct ptrace_child *child,
                                   unsigned long sysno) {
    s390_regs regs;
    struct iovec reg_iovec = {
        .iov_base = &regs,
        .iov_len = sizeof(regs)
    };

    if (ptrace_command(child, PTRACE_GETREGSET, NT_PRSTATUS, &reg_iovec) < 0)
        return -1;

    regs.gprs[2] = sysno;
    return ptrace_command(child, PTRACE_SETREGSET, NT_PRSTATUS, &reg_iovec);
}

static inline int arch_save_syscall(struct ptrace_child *child) {
    return 0;
}

static inline int arch_restore_syscall(struct ptrace_child *child) {
    return 0;
}
