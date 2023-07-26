#ifndef __BONDING_CLI_H__
#define __BONDING_CLI_H__

#include <structopt/app.hpp>

namespace bonding::cli {

    struct Args {
        /* Activate debug mode */
        std::optional<bool> debug = false;

        /* Command to execute inside the container */
        std::string command;

        /* User ID to create inside the container */
        std::uint32_t uid;

        /* Directory to mount as root of the container */
        std::string mount_dir;
    };

};

/** Command line argument parser generated by structopt */
STRUCTOPT(cli::Args, debug, command, uid, mount_dir);

#endif /* __BONDING_CLI_H__ */