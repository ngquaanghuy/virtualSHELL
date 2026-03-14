#ifndef COMMANDS_LOADER_H
#define COMMANDS_LOADER_H

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "../include/SandboxManager.h"

class Command {
public:
    virtual ~Command() = default;
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getUsage() const = 0;
    virtual int getMinArgs() const = 0;
    virtual int getMaxArgs() const = 0;
    virtual bool requiresRoot() const = 0;
    virtual int execute(const std::vector<std::string>& args, SandboxManager* sandbox) = 0;
};

#define DECLARE_COMMAND(classname) \
    class classname : public Command { \
    public: \
        std::string getName() const override; \
        std::string getDescription() const override; \
        std::string getUsage() const override; \
        int getMinArgs() const override; \
        int getMaxArgs() const override; \
        bool requiresRoot() const override; \
        int execute(const std::vector<std::string>& args, SandboxManager* sandbox) override; \
    };

#define IMPLEMENT_COMMAND(classname) \
    std::string classname::getName() const { return ""; } \
    std::string classname::getDescription() const { return ""; } \
    std::string classname::getUsage() const { return ""; } \
    int classname::getMinArgs() const { return 0; } \
    int classname::getMaxArgs() const { return 0; } \
    bool classname::requiresRoot() const { return false; } \
    int classname::execute(const std::vector<std::string>& args, SandboxManager* sandbox) { return 0; }

#include "Cmd_ls.h"
#include "Cmd_cd.h"
#include "Cmd_pwd.h"
#include "Cmd_mkdir.h"
#include "Cmd_rmdir.h"
#include "Cmd_rm.h"
#include "Cmd_touch.h"
#include "Cmd_cat.h"
#include "Cmd_cp.h"
#include "Cmd_mv.h"
#include "Cmd_ln.h"
#include "Cmd_chmod.h"
#include "Cmd_chown.h"
#include "Cmd_tree.h"
#include "Cmd_find.h"
#include "Cmd_du.h"
#include "Cmd_df.h"
#include "Cmd_stat.h"
#include "Cmd_wc.h"
#include "Cmd_head.h"
#include "Cmd_tail.h"
#include "Cmd_sudo.h"
#include "Cmd_apt.h"
#include "Cmd_pkg.h"
#include "Cmd_uname.h"
#include "Cmd_whoami.h"
#include "Cmd_hostname.h"
#include "Cmd_date.h"
#include "Cmd_cal.h"
#include "Cmd_uptime.h"
#include "Cmd_id.h"
#include "Cmd_groups.h"
#include "Cmd_passwd.h"
#include "Cmd_su.h"
#include "Cmd_which.h"
#include "Cmd_whereis.h"
#include "Cmd_type.h"
#include "Cmd_alias.h"
#include "Cmd_unalias.h"
#include "Cmd_env.h"
#include "Cmd_export.h"
#include "Cmd_unset.h"
#include "Cmd_exit.h"
#include "Cmd_logout.h"
#include "Cmd_shutdown.h"
#include "Cmd_reboot.h"
#include "Cmd_halt.h"
#include "Cmd_poweroff.h"
#include "Cmd_systemctl.h"
#include "Cmd_service.h"
#include "Cmd_clear.h"
#include "Cmd_reset.h"
#include "Cmd_ping.h"
#include "Cmd_curl.h"
#include "Cmd_wget.h"
#include "Cmd_ssh.h"
#include "Cmd_scp.h"
#include "Cmd_ftp.h"
#include "Cmd_netstat.h"
#include "Cmd_ifconfig.h"
#include "Cmd_ip.h"
#include "Cmd_nslookup.h"
#include "Cmd_dig.h"
#include "Cmd_host.h"
#include "Cmd_traceroute.h"
#include "Cmd_echo.h"
#include "Cmd_printf.h"
#include "Cmd_grep.h"
#include "Cmd_sed.h"
#include "Cmd_awk.h"
#include "Cmd_sort.h"
#include "Cmd_uniq.h"
#include "Cmd_cut.h"
#include "Cmd_tr.h"
#include "Cmd_rev.h"
#include "Cmd_tee.h"
#include "Cmd_ps.h"
#include "Cmd_top.h"
#include "Cmd_kill.h"
#include "Cmd_killall.h"
#include "Cmd_pkill.h"
#include "Cmd_bg.h"
#include "Cmd_fg.h"
#include "Cmd_jobs.h"
#include "Cmd_nice.h"
#include "Cmd_nohup.h"
#include "Cmd_watch.h"
#include "Cmd_time.h"
#include "Cmd_help.h"
#include "Cmd_man.h"
#include "Cmd_history.h"
#include "Cmd_version.h"
#include "Cmd_info.h"
#include "Cmd_banner.h"
#include "Cmd_cowsay.h"
#include "Cmd_yes.h"
#include "Cmd_sleep.h"
#include "Cmd_true.h"
#include "Cmd_false.h"
#include "Cmd_test.h"
#include "Cmd_read.h"
#include "Cmd_source.h"
#include "Cmd_hash.h"
#include "Cmd_return.h"
#include "Cmd_local.h"
#include "Cmd_declare.h"
#include "Cmd_readonly.h"
#include "Cmd_typeset.h"
#include "Cmd_shift.h"
#include "Cmd_set.h"
#include "Cmd_trap.h"
#include "Cmd_umask.h"
#include "Cmd_wait.h"
#include "Cmd_xargs.h"
#include "Cmd_save.h"
#include "Cmd_load.h"

#endif
