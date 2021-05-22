#include <experimental/filesystem>
#include <string>

#include "../include/linux_parser.h"
#include "gtest/gtest.h"
#include "helper/test_helpers.h"

#define EXPECT_FLOAT_EQ_WITH_DELTA(val1, val2, delta) \
        EXPECT_TRUE((abs(val1 - val2)) < delta)

using namespace std;

class LinuxParserTest : public ::testing::Test {
 public:
  const int kPidOne{1015};
  const int kPidTwo{1016};
  const int kPidThree{1017};

  const string kTestResourcesDirectory{"Testing/resources/"};

  const string kProcDirectory{kTestResourcesDirectory + "proc/"};
  const string kCmdlineFilename{"/cmdline"};
  const string kCpuinfoFilename{"/cpuinfo"};
  const string kStatusFilename{"/status"};
  const string kStatPath{kTestResourcesDirectory+ "stat"};
  const string kStatFilename{"/stat"};
  const string kUptimePath{kTestResourcesDirectory + "uptime"};
  const string kMeminfoPath{kTestResourcesDirectory + "meminfo"};
  const string kKernelPath{resolvePath(kTestResourcesDirectory + "version")};
  const string kOSPath{resolvePath(kTestResourcesDirectory + "os-release")};
  const string kPasswordPath{"/etc/passwd"};
};

TEST_F(LinuxParserTest, TestOperatingSystem){
  string expected_os = "Pretty Name of a OS";
  string actual_os = LinuxParser::OperatingSystem(kOSPath);

  EXPECT_EQ(expected_os, actual_os);
}

TEST_F(LinuxParserTest, TestKernel){
  string expected_kernel = "5.4.72-microsoft-standard-WSL2";
  string actual_kernel = LinuxParser::Kernel(kKernelPath);

  EXPECT_EQ(expected_kernel, actual_kernel);
}

TEST_F(LinuxParserTest, TestMemoryUtilization){
  float expected_memory_usage = 0.2214;
  float actual_memory_usage = LinuxParser::MemoryUtilization(kMeminfoPath);

  EXPECT_FLOAT_EQ_WITH_DELTA(expected_memory_usage, actual_memory_usage, 0.001);
}

TEST_F(LinuxParserTest, TestUpTime){
  long expected_uptime = 1200;
  long actual_uptime = LinuxParser::UpTime(kUptimePath);

  EXPECT_LE(expected_uptime, actual_uptime);
}

TEST_F(LinuxParserTest, TestTotalProcesses){
  int expected_total_processes = 5000;
  int actutal_total_processes = LinuxParser::TotalProcesses(kStatPath);

  EXPECT_LE(expected_total_processes, actutal_total_processes);
}

TEST_F(LinuxParserTest, TestRunningProcesses){
  int expected_running_processes = 300;
  int actual_running_processes = LinuxParser::RunningProcesses(kStatPath);

  EXPECT_EQ(expected_running_processes, actual_running_processes);
}

TEST_F(LinuxParserTest, TestJiffies){
  long expected_jiffies = 3370364;
  long actual_jiffies = LinuxParser::Jiffies(kStatPath);

  EXPECT_EQ(expected_jiffies, actual_jiffies);
}

TEST_F(LinuxParserTest, TestActiveJiffies){
  long expected_active_jiffies = 51246;
  long actual_active_jiffies = LinuxParser::ActiveJiffies(kStatPath);

  EXPECT_EQ(expected_active_jiffies, actual_active_jiffies);
}

TEST_F(LinuxParserTest, TestIdleJiffies){
  long expected_idle_jiffies = 3319118;
  long actual_idle_jiffies = LinuxParser::IdleJiffies(kStatPath);

  EXPECT_EQ(expected_idle_jiffies, actual_idle_jiffies);
}

TEST_F(LinuxParserTest, TestCommand){
  string expected_command = "/command";
  string actual_command = LinuxParser::Command(kPidOne, kTestResourcesDirectory);

  EXPECT_EQ(expected_command, actual_command);
}

TEST_F(LinuxParserTest, TestUid){
  string expected_uid = "0";
  string actual_uid = LinuxParser::Uid(kPidOne, kTestResourcesDirectory);

  EXPECT_EQ(expected_uid, actual_uid);
}

TEST_F(LinuxParserTest, TestStarttimeForPid){
  long expected_start_time = 46397;
  long actual_start_time = LinuxParser::StartTime(kPidOne, kTestResourcesDirectory);

  EXPECT_EQ(expected_start_time, actual_start_time);
}

TEST_F(LinuxParserTest, TestRam){
  string expected_ram = "1.632";
  string actual_ram = LinuxParser::Ram(kPidOne, kTestResourcesDirectory);

  EXPECT_EQ(expected_ram, actual_ram);
}

TEST_F(LinuxParserTest, TestActiveJiffiesForPid){
  long expected_active_jiffies = 5000;
  long actual_active_jiffes = LinuxParser::ActiveJiffies(
      kPidOne, kTestResourcesDirectory, kStatFilename);

  EXPECT_EQ(expected_active_jiffies, actual_active_jiffes);
}

TEST_F(LinuxParserTest, TestCpuUtilization){
  vector<int> pids{kPidTwo, kPidThree};
  vector<string> cpu_utilizations = LinuxParser::CpuUtilization(pids, kTestResourcesDirectory, kStatFilename, kUptimePath, 10);

  string expected_cpu_utilization_one = "PID 1016: 59";
  string expected_cpu_utilization_two = "PID 1017: 29";

  string actual_cpu_utilization_one = cpu_utilizations[0];
  string actual_cpu_utilization_two = cpu_utilizations[1];

  EXPECT_EQ(expected_cpu_utilization_one, actual_cpu_utilization_one);
  EXPECT_EQ(expected_cpu_utilization_two, actual_cpu_utilization_two);
}