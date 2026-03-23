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
    child->regs.gprs[2] = child->regs.orig_gpr2;
}

static inline int arch_set_syscall(struct ptrace_child *child,
                                   unsigned long sysno) {
    return ptrace_command(child, PTRACE_POKEUSER, PT_GPR2, sysno);
}

static inline int arch_save_syscall(struct ptrace_child *child) {
    child->saved_syscall = ptrace_command(child, PTRACE_PEEKUSER, PT_GPR2);
    if (child->error)
        return -1;
    return 0;
}

static inline int arch_restore_syscall(struct ptrace_child *child) {
    return 0;
}
