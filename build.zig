const std = @import("std");
const system = @import("system");

pub fn build(b: *std.Build) void {
  const target = b.standardTargetOptions(.{});
  const optimize = b.standardOptimizeOption(.{});

  // BlizzardCore library
  const bc = b.addStaticLibrary(.{
    .name = "bc",
    .target = target,
    .optimize = optimize
  });
  // Link C++ standard library
  bc.linkLibCpp();
  // Add system detection defines
  system.add_defines(bc);

  // Publicly link mem
  const mem = b.dependency("mem", .{});
  bc.linkLibrary(mem.artifact("mem"));

  bc.addIncludePath(b.path("."));

  bc.addCSourceFiles(.{
    .files = &.{
      "bc/file/path/Path.cpp",
      "bc/file/path/Posix.cpp",
      "bc/file/File.cpp",
      "bc/file/Filesystem.cpp",

      "bc/lock/Atomic.cpp",

      "bc/os/CommandLine.cpp",
      "bc/os/File.cpp",
      "bc/os/Path.cpp",

      "bc/system/file/posix/Stacked.cpp",
      "bc/system/file/posix/System_File.cpp",
      "bc/system/file/win/Stacked.cpp",
      "bc/system/file/win/System_File.cpp",
      "bc/system/file/win/WinFile.cpp",
      "bc/system/file/Stacked.cpp",
      "bc/system/System_Debug.cpp",
      "bc/system/System_Lock.cpp",
      "bc/system/System_Thread.cpp",
      "bc/system/System_Time.cpp",

      "bc/time/Time.cpp",

      "bc/Debug.cpp",
      "bc/Lock.cpp",
      "bc/Memory.cpp",
      "bc/Process.cpp",
      "bc/String.cpp",
      "bc/Thread.cpp",
    },

    .flags = &.{"-std=c++11"}
  });

  bc.installHeadersDirectory(b.path("bc"), "bc", .{ .include_extensions = &.{"hpp"} });

  // BcTest executable
  const bc_test_exe = b.addExecutable(.{
    .name = "BcTest",
    .target = target,
    .optimize = optimize
  });
  // Link C++ standard library
  bc_test_exe.linkLibCpp();
  // Add system detection defines
  system.add_defines(bc_test_exe);

  bc_test_exe.linkLibrary(mem.artifact("mem"));
  bc_test_exe.linkLibrary(bc);

  bc_test_exe.addIncludePath(b.path("."));

  bc_test_exe.addCSourceFiles(.{
    .files = &.{
      "test/Lock.cpp",
      "test/Memory.cpp",
      "test/Process.cpp",
      "test/String.cpp",
      "test/Test.cpp",
      "test/Thread.cpp",
      "test/Time.cpp",
    },

    .flags = &.{"-std=c++11"}
  });

  b.installArtifact(bc_test_exe);
  b.installArtifact(bc);
}
