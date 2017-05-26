/*
 * Support routine for configuring link layer address
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#elif defined(_MSC_VER)
#include "config-msvc.h"
#endif

#include "syshead.h"
#include "error.h"
#include "misc.h"
#include "run_command.h"
#include "lladdr.h"
#include "proto.h"

#if defined(ENABLE_NDM_INTEGRATION)
#include <ndm/feedback.h>
#include "ndm.h"
#endif /* if defined(ENABLE_NDM_INTEGRATION) */

int
set_lladdr(openvpn_net_ctx_t *ctx, const char *ifname, const char *lladdr,
           const struct env_set *es)
{
#if !defined(ENABLE_NDM_INTEGRATION)
    int r;
#endif /* if defined(ENABLE_NDM_INTEGRATION) */

    if (!ifname || !lladdr)
    {
        return -1;
    }

#if defined(ENABLE_NDM_INTEGRATION)
    {
         char buf[1024];

        memset(buf, 0, sizeof(buf));

        snprintf(buf, sizeof(buf), "%s%s/%s",
            NDM_OPENVPN_DIR,
            NDM_INSTANCE_NAME,
            NDM_FEEDBACK_NETWORK);

        const char *args[] =
        {
            buf,
            NDM_INSTANCE_NAME,
            NDM_LLADDR,
            NULL
        };

        if( !ndm_feedback(
                NDM_FEEDBACK_TIMEOUT_MSEC,
                args,
                "%s=%s" NESEP_
                "%s=%s",
                "dev", ifname,
                "lladdr", lladdr) )
        {
            msg(M_FATAL, "Unable to communicate with NDM core (lladdr)");

            return 0;
        }

        return 1;
    }
#else /* if defined(ENABLE_NDM_INTEGRATION) */
#if defined(TARGET_LINUX)
    uint8_t addr[OPENVPN_ETH_ALEN];

    sscanf(lladdr, MAC_FMT, MAC_SCAN_ARG(addr));
    r = (net_addr_ll_set(ctx, ifname, addr) == 0);
#else /* if defined(TARGET_LINUX) */
    struct argv argv = argv_new();
#if defined(TARGET_SOLARIS)
    argv_printf(&argv,
                "%s %s ether %s",
                IFCONFIG_PATH,
                ifname, lladdr);
#elif defined(TARGET_OPENBSD)
    argv_printf(&argv,
                "%s %s lladdr %s",
                IFCONFIG_PATH,
                ifname, lladdr);
#elif defined(TARGET_DARWIN)
    argv_printf(&argv,
                "%s %s lladdr %s",
                IFCONFIG_PATH,
                ifname, lladdr);
#elif defined(TARGET_FREEBSD)
    argv_printf(&argv,
                "%s %s ether %s",
                IFCONFIG_PATH,
                ifname, lladdr);
#else  /* if defined(TARGET_SOLARIS) */
    msg(M_WARN, "Sorry, but I don't know how to configure link layer addresses on this operating system.");
    return -1;
#endif /* if defined(TARGET_SOLARIS) */
    argv_msg(M_INFO, &argv);
    r = openvpn_execve_check(&argv, es, M_WARN, "ERROR: Unable to set link layer address.");
    argv_free(&argv);
#endif /* if defined(TARGET_LINUX) */

    if (r)
    {
        msg(M_INFO, "TUN/TAP link layer address set to %s", lladdr);
    }

    return r;
#endif /* if defined(ENABLE_NDM_INTEGRATION) */
}
