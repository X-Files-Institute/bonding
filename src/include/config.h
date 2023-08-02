#ifndef __BONDING_CONFIG_H__
#define __BONDING_CONFIG_H__

#include "cli.h"
#include "error.h"
#include "hostname.h"
#include "mount.h"
#include "namespace.h"
#include "result.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace bonding::config
{

  /** Extract the command line arguments into this class
   ** and initialize a Container struct that will have to perform
   ** the container work. */
  class Container_Options
  {
   public:
    static Result<std::pair<Container_Options, std::pair<int, int>>, error::Err>
    make(const std::string & command,
         const std::string & mount_dir,
         const uint32_t uid,
         const std::string & hostname) noexcept
    {
      const std::pair<int, int> sockets = bonding::cli::generate_socketpair().unwrap();
      const std::vector<std::string> argv =
        parse_argv(command).expect("Cannot parse command arguments");

      return Ok(
        std::make_pair(Container_Options(argv,
                                         argv.at(0),
                                         mount_dir,
                                         uid,
                                         sockets.second,
                                         bonding::hostname::Hostname(hostname),
                                         bonding::mounts::Mount(mount_dir, hostname)),
                       sockets));
    }

    Container_Options()
      : m_mount_dir("")
      , m_uid(-1)
      , m_argv({})
      , m_path("")
      , m_raw_fd(0)
      , m_hostname(bonding::hostname::Hostname(""))
      , m_mount(bonding::mounts::Mount("", ""))
      , m_namespace(ns::Namespace(-1, -1))
    {
      std::terminate();
    }

    inline const Container_Options
    operator=(const Container_Options & container_options) const noexcept
    {
      return container_options;
    }

   private:
    Container_Options(const std::vector<std::string> argv,
                      const std::string path,
                      const std::string mount_dir,
                      const uint32_t uid,
                      const int raw_fd,
                      const bonding::hostname::Hostname hostname,
                      const bonding::mounts::Mount mount)
      : m_argv(argv)
      , m_path(path)
      , m_mount_dir(mount_dir)
      , m_uid(uid)
      , m_raw_fd(raw_fd)
      , m_hostname(hostname)
      , m_mount(mount)
      , m_namespace(ns::Namespace(raw_fd, uid))
    {
    }

   private:
    static Result<std::vector<std::string>, Unit>
    parse_argv(const std::string argv) noexcept;

   public:
    /** The path of the binary executable script to execute inside the container. */
    const std::string m_path;

    /** The path of the directory to use as a root inside container. */
    const std::string m_mount_dir;

    /** The ID of the user inside the container.
     ** An ID of 0 means it’s root (administrator). */
    const uint32_t m_uid;

    /** The full arguments passed (including the path option) into the commandline. */
    const std::vector<std::string> m_argv;

    /** socket for IPC */
    const int m_raw_fd;

    /** A hostname is what identifies machine compared
     ** to every other living on the same network. */
    const bonding::hostname::Hostname m_hostname;

    const bonding::mounts::Mount m_mount;
    const bonding::ns::Namespace m_namespace;
  };

};

#endif /* __BONDING_CONFIG_HPP__ */