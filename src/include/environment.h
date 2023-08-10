/** Copyright (C) 2023 Muqiu Han <muqiu-han@outlook.com> */

#ifndef __BONDING_CHECK_H__
#define __BONDING_CHECK_H__

#include "result.hpp"
#include "unix.h"
#include <map>
#include <sys/utsname.h>
#include <vector>

namespace bonding::environment
{
  class Kernel
  {
   private:
    const utsname host;

   private:
    static Result<std::pair<int, int>, error::Err> parse_version(const utsname & host);

   public:
    Kernel()
      : host(unix::Utsname::get().unwrap())
      , version(parse_version(host).unwrap())
      , machine(host.machine)
      , release(host.release)
      , sysname(host.sysname)
      , node_name(host.nodename)
      , domain_name(host.domainname)
    {
      spdlog::debug("Get host machine information...");
      spdlog::debug("System: {}, Machine: {}, Release: {}, Node: {}, Domain: {}",
                    sysname,
                    machine,
                    release,
                    node_name,
                    domain_name);
    }

   public:
    std::pair<int, int> version;
    const std::string domain_name;
    const std::string machine;
    const std::string release;
    const std::string sysname;
    const std::string node_name;
  };

  class CgroupsV1
  {
   private:
    inline static const std::string PATH = "/sys/fs/cgroup/";

   private:
    static Result<std::map<std::string, bool>, error::Err>
    check_supported_controller() noexcept;

   private:
    inline static const std::vector<std::string> controllers = {
      "blkio",   "blkio.bfq", "cpuacct", "cpuset",           "freezer",    "memory",
      "net_cls", "net_prio",  "pids",    "systemd",          "cpu",        "cpu,cpuacct",
      "devices", "hugetlb",   "misc",    "net_cls,net_prio", "perf_event", "rdma",
      "unified"
    };

   public:
    inline static const std::map<std::string, bool> supported_controllers =
      check_supported_controller().unwrap();
  };

  class Info
  {
   public:
    inline static const Kernel kernel;
  };
};

#endif /* __BONDING_CHECK_H__ */