/* Signals active during readline prompt */
void setup_sigreadline_handlers(void)
{
    struct sigaction sa;

    // Ctrl-C handler
    sa.sa_handler = sigint_readline_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Ignore Ctrl-\ (SIGQUIT)
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

/* Signals active during command execution */
void setup_sigexec_handlers(void)
{
    struct sigaction sa;

    // Minimal Ctrl-C handler for parent
    sa.sa_handler = sigint_minimal;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    // Ignore SIGQUIT in parent
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
