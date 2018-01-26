#!/usr/bin/env python

# File: manage.py
# Creation date: 2017-02-05
# Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com>
# Description:
#  The project management script

import os
import sys
import argparse
import datetime
import subprocess
import platform
import pickle

# Colors for beautiful output
CLR_YELLOW = "\x1b[38;5;226m"
CLR_GRAY = "\x1b[38;5;239m"
CLR_RESET = "\x1b[0m"

# Version
VERSION_MAJOR = "0"
VERSION_MINOR = "0"
VERSION_PATCH = "5"
VERSION_EXTRA = "default"
VERSION = "{}.{}.{} ({})".format(
    VERSION_MAJOR,
    VERSION_MINOR,
    VERSION_PATCH,
    VERSION_EXTRA)

# Paths
ROOT_DIR = os.path.abspath("{}/../..".format(sys.argv[0]))
SRC_ROOT_DIR = "{}/src".format(ROOT_DIR)
TMP_ROOT_DIR = "{}/tmp".format(ROOT_DIR)
MAKEFILES_DIR = "{}/mk".format(ROOT_DIR)
USER_CONF_DIR = "~/.gsh"
SYS_CONF_DIR = "/etc/gsh"
DEFAULT_CONF_DIR = "{}/resources/etc/gsh".format(ROOT_DIR)
DEFAULT_MODULES_SEARCH_PATH = "/usr/share/gsh/modules"
RESOURCES_ROOT_DIR = "{}/resources".format(ROOT_DIR)
PARSER_TESTS_RESOURCES_DIR = "{}/tests/parser".format(RESOURCES_ROOT_DIR)
LEXER_TESTS_RESOURCES_DIR = "{}/tests/lexer".format(RESOURCES_ROOT_DIR)
FILE_WITH_HELLO_IN_IT = "{}/resources/tests/platform/file_with_word_hello.txt".format(
    ROOT_DIR)
EXEC = "{}/tmp/taste".format(ROOT_DIR)


class LDWrapper(object):
    """
    The wrapper around the linker. Supports only gcc at the moment

    Attributes
    ----------
    ld: str
      The command to run the linker
    flags: str[]
      The flags to pass to the linker
    """

    ld = "gcc"
    flags = ["-lc", "-lm"]

    def __init__(self, debug):
        """
        Initialize the wrapper.

        Parameters
        ----------
        debug: bool
          The "debug" flag. If set to True, the debug flags would be added.
        """

        if debug:
            self.flags += ["-g", "-ggdb"]

    def get_ld(self):
        return self.ld

    def get_flags(self):
        return self.flags


class CCWrapper(object):
    """
    The compiler wrapper.

    Attributes
    ----------
    debug: bool
      Flag that indicates whenether to compile with debug flags or not
    test: bool
      Flag that indicates whenether to compile with tests or not
    bench: bool
      Flag that indicates whenether to compile benchmarks or not
    std_flags: str[]
      The list of std flags (e.g: -pedantic, -std=c11)
    warn_flags: str[]
      The list of warning flags
    include_flags: str[]
      The list of include flags
    optimization_flags: str[]
      The list of optimization flags
    debug_flags: str[]
      The list of debug flags
    def_flags: str[]
      The list of flags that define something
    flags: str[]
      The list of all flags
    """

    def __init__(self, debug, test, bench):
        """
        Initialize the wrapper.

        Parameters
        ----------
        debug : bool
          The "debug" flag. If set, compiler will set the debug flags
          when compiling.
        test : bool
          The "test" flag. If set, compiler will compile the tests.
        bench : bool
          The "bench" flag. If set, compiler will compile the benchmarks.
        """

        self.debug = debug
        self.test = test
        self.bench = bench
        self.cc = "gcc"
        self.std_flags = ["-std=c11"]

        self.warn_flags = [
            "-Wextra",
            "-Werror",
            "-Wformat=2",
            "-Wno-missing-braces",
            "-Wno-missing-field-initializers",
            "-Wswitch-default",
            "-Wcast-align",
            "-Wpointer-arith",
            "-Wbad-function-cast",
            "-Wstrict-overflow=5",
            "-Wstrict-prototypes",
            "-Winline",
            "-Wundef",
            "-Wnested-externs",
            "-Wshadow",
            "-Wunreachable-code",
            "-Wlogical-op",
            "-Wfloat-equal",
            "-Wstrict-aliasing=2",
            "-Wredundant-decls",
            "-Wold-style-definition",
            "-fno-omit-frame-pointer",
            "-ffloat-store",
            "-fno-common",
            "-fstrict-aliasing"
        ]

        self.include_flags = ["-I{}".format(s) for s in [
            ROOT_DIR,
            SRC_ROOT_DIR
        ]]

        if (debug):
            self.optimization_flags = ["-O0"]
            self.debug_flags = ["-g", "-ggdb"]
        else:
            self.optimization_flags = ["-O3"]
            self.debug_flags = []

        def_flags_simple = ["-D_GNU_SOURCE"]
        def_flags_vals = ["-DVERSION=\"{}\"".format(VERSION)]
        def_flags_paths = [
            "-DUSER_CONF_DIR=\"{}\"".format(USER_CONF_DIR),
            "-DSYS_CONF_DIR=\"{}\"".format(SYS_CONF_DIR),
            "-DDEFAULT_CONF_DIR=\"{}\"".format(DEFAULT_CONF_DIR),
            "-DDEFAULT_MODULES_SEARCH_PATH=\"{}\"".format(
                DEFAULT_MODULES_SEARCH_PATH),
            "-DPARSER_TESTS_RESOURCES_DIR=\"{}\"".format(
                PARSER_TESTS_RESOURCES_DIR),
            "-DLEXER_TESTS_RESOURCES_DIR=\"{}\"".format(
                LEXER_TESTS_RESOURCES_DIR),
            "-DFILE_WITH_HELLO_IN_IT_PATH=\"{}\"".format(FILE_WITH_HELLO_IN_IT)
        ]

        def_flags_debug = [
            "-DDEBUG={}".format(int(self.debug)),
            "-DDEBUG_MEMORY={}".format(int(self.debug)),
            "-DDEBUG_VERBOSE=1"
        ]

        def_flags_test = [
            "-DTEST={}".format(int(self.test)),
            "-DDICT_TEST={}".format(int(self.test)),
            "-DXVEC_STREAM_TEST={}".format(int(self.test))
        ]

        def_flags_bench = [
            "-DBENCH={}".format(int(self.bench)),
            "-DDICT_BENCH={}".format(int(self.bench))
        ]

        self.def_flags = (def_flags_simple + def_flags_vals + def_flags_paths +
                          def_flags_debug + def_flags_test + def_flags_bench)

        self.flags = (
            self.std_flags + self.warn_flags + self.include_flags +
            self.optimization_flags + self.debug_flags + self.def_flags)

        return None

    def get_cc(self):
        return self.cc

    def get_include_flags(self):
        return self.include_flags

    def get_flags(self):
        return self.flags


class Builder(object):
    """
    The builder.

    The purpose of a builder is to build a project.
    """

    """
    The last build time file path.
    """
    LAST_BUILD_TIME_FILE = "{}/last_build_time.pickle".format(TMP_ROOT_DIR)

    """
    The dependencies file path.
    """
    DEPS_FILE = "{}/deps.pickle".format(RESOURCES_ROOT_DIR)

    # TODO: Move to the linker
    """
    Static libraries to link
    """
    STATIC_LIBS_TO_LINK = [
        "{}/deps/sil/tmp/libsil.a".format(ROOT_DIR)
    ]

    def __init__(self, debug, test, bench):
        """
        Initialize the builder.

        Parameters
        ----------
        debug : bool
          The "debug" flag. If set, builder will set the debug flags
          when compiling.
        test : bool
          The "test" flag. If set, builder will compile the tests.
        bench : bool
          The "bench" flag. If set, builder will compile the benchmarks.
        """

        self.debug = debug
        self.test = test
        self.bench = bench
        self.last_build_time = self.load_last_build_time()
        self.deps = self.load_deps()
        self.ldwrapper = LDWrapper(debug)
        self.ccwrapper = CCWrapper(debug, test, bench)

    def load_last_build_time(self):
        """
        Return the last time project was built.

        Returns
        -------
        datetime.datetime
          The time since last project build
        """

        # If there is no saved file with the last build time
        if not os.path.exists(self.LAST_BUILD_TIME_FILE):
            # Create new file
            f = open(self.LAST_BUILD_TIME_FILE, "w")
            f.close()
            return datetime.datetime.fromtimestamp(0)
        # Load the file contents
        try:
            with open(self.LAST_BUILD_TIME_FILE, "rb") as f:
                return pickle.load(f)
        except EOFError:
            return datetime.datetime.fromtimestamp(0)

    def save_last_build_time(self, time):
        """
        Save the given build time into the file.
        """
        with open(self.LAST_BUILD_TIME_FILE, "wb") as f:
            pickle.dump(time, f)

    def load_deps(self):
        """
        Load the saved dependency graph and return it as a Python dictionary.

        Returns
        -------
        dict
          The dictionary that represents the dependency graph
          (src_file_path: [dep1, dep2, ..., depn]).
        """

        if not os.path.exists(self.DEPS_FILE):
            # Create a new file
            f = open(self.DEPS_FILE, "w")
            f.close()
            return {}

        # Load the file contents
        try:
            with open(self.DEPS_FILE, "rb") as f:
                return pickle.load(f)
        except EOFError:
            return {}

    def path_was_changed_after_last_build(self, path):
        """
        Return True if file at the given path was changed
        after the last compilation

        Returns
        -------
        bool
          True if the given source file was changed after the last compilation.
          False otherwise.
        """
        try:
            mtime = os.path.getmtime(path)
        except OSError:
            mtime = 0
        return datetime.datetime.fromtimestamp(mtime) > self.last_build_time

    @staticmethod
    def parse_dep_generator_output(output):
        """
        Parse an output generated by the GCC dependency generator
        and return a list of dependency paths.

        Returns
        -------
        list:
          The list containing the paths dependencies (paths of other files)
          from a given dependency generator output.
        """

        paths = []
        # For each character in output
        i = 0
        while (i < len(output)):
            if (output[i] == ":"):
                # Skip the colon ':'
                i += 1
                while i < len(output):
                    dep_path = ""
                    while (i < len(output)) and (output[i] != "\\") and (output[i] != " "):
                        if output[i] != " ":
                            dep_path += output[i]
                        i += 1
                    # If path is good
                    if len(dep_path) > 1:
                        paths.append(dep_path)
                    i += 1
                    # If we seen an line feed character '\n'
                    if (i < len(output)) and (output[i] == "n"):
                        i += 1
            i += 1
        return paths

    @staticmethod
    def src_path_to_obj_path(path):
        """
        Map the given source file path onto the object file path

        Returns
        -------
        str
          The mapped object file
        """
        return path.replace(".c", ".o").replace("src", "tmp")

    def src_file_is_up_to_date(self, path):
        """
        Return true if the source file at given path
        is need to be recompiled.

        Returns
        -------
        bool
          True if given source file is up-to-date. False otherwise
        """

        # If there is no compiled '*.o' file at all,
        # surely we need to recompile it
        if not os.path.exists(Builder.src_path_to_obj_path(path)):
            return False

        # if path was changed after the last compilation: return false
        if self.path_was_changed_after_last_build(path):
            return False

        # If there is no generated dependencies for this file
        if not path in self.deps:
            return False

        # For each dependency path of a given path:
        for dep_path in self.deps[path]:
            # If dep_path was changed after the last compilation: return false
            if self.path_was_changed_after_last_build(dep_path):
                return False
        return True

    # TODO: Name this function better
    def collect_needed_files(self):
        """
        Collect all source/object/dep file paths
        in the whole project and return tuple
        """

        srcs = []
        objs = []
        for root, dirs, filenames in os.walk(SRC_ROOT_DIR):
            tmp_root = root.replace("/src", "/tmp")
            for f in filenames:
                if f.endswith(".c"):
                    srcs.append("{}/{}".format(root, f))
                    if not os.path.exists(tmp_root):
                        os.makedirs(tmp_root)
                    objs.append(
                        "{}/{}".format(tmp_root, f.replace(".c", ".o")))
        return srcs, objs

    def src_file_gen_deps(self, path):
        """
        Generate a list of dependencies from a given .c file,
        and return the list of dependencies as the Python list.

        Returns
        -------
        list:
          The list of generated dependencies
        """

        # Create the command
        cmd = [self.ccwrapper.get_cc()]
        cmd += self.ccwrapper.get_include_flags()
        cmd += ["-MM", path]
        # Retreive the dependencies string
        output = subprocess.check_output(cmd)
        # Convert this string into the list
        paths = Builder.parse_dep_generator_output(str(output))
        print("{}GEN {}\"{}\"{}".format(CLR_YELLOW, CLR_GRAY, path, CLR_RESET))
        return paths

    def gen_deps(self):
        """
        Generate dependencies for the project source files.

        This function will generate the file named `self.DEPS_FILE`,
        and pickle the dependency graph into it.
        """

        srcs, _ = self.collect_needed_files()
        deps = {}
        for src_file in srcs:
            deps[src_file] = self.src_file_gen_deps(src_file)

        with open(self.DEPS_FILE, "wb") as f:
            pickle.dump(deps, f)
        print("Dependencies were successfuly generated")

    def compile(self):
        """
        Compile the whole project.

        Basically, what this function does, is it turns all source
        files into the object files with the corresponding name.

        Returns
        -------
        bool
          True if anything was changed (if at least one
          source file was not up-to-date)
        """

        is_anything_changed = False
        # Retreive source and object files to compile
        srcs, objs = self.collect_needed_files()
        # For each source file
        for i in range(len(srcs)):
            # If this file is not up to date
            if not self.src_file_is_up_to_date(srcs[i]):
                is_anything_changed = True
                # Print the beautiful message
                print(
                    "{}CC {}{}{}".format(CLR_YELLOW, CLR_GRAY, srcs[i], CLR_RESET))
                # Compile
                cmd = [self.ccwrapper.get_cc()]
                cmd += self.ccwrapper.get_flags()
                cmd += ["-c", "-o", objs[i], srcs[i]]
                if (subprocess.call(cmd) != 0):
                    return False
        return is_anything_changed

    def link(self):
        """
        Link all the object files with needed libraries
        and output a single executable.

        Returns
        -------
        bool
          True if linkage was successfull (zero exit code).
          False otherwise
        """

        _, objs = self.collect_needed_files()

        # Print the beautiful message
        print("{}LD {}{}{}".format(CLR_YELLOW, CLR_GRAY, EXEC, CLR_RESET))
        cmd = [self.ldwrapper.get_ld()]
        cmd += self.ldwrapper.get_flags()
        cmd += ["-o", EXEC]
        cmd += objs
        cmd += self.STATIC_LIBS_TO_LINK
        if (subprocess.call(cmd) != 0):
            return False
        return True

    def build(self):
        """Build the project."""

        # Clear the screen
        clear_screen()

        # Save the build start time
        build_start_time = datetime.datetime.now()

        # Compile
        if not self.compile():
            return

        # Link
        if not self.link():
            return

        build_end_time = datetime.datetime.now()
        build_time = build_end_time - build_start_time
        print("Build finished in {} ms".format(build_time.microseconds))
        self.last_build_time = build_end_time
        self.save_last_build_time(build_end_time)


def exec_mk(name):
    """
    Execute the given .mk file in the project's makefiles directory.

    Parameters
    ----------
    name: str
      name of the Make file to execute
    """
    clear_screen()
    subprocess.call(["make", "-f", "{}/{}.mk".format(MAKEFILES_DIR, name)])
    return None


def clean():
    """Clean the project tree."""
    exec_mk("clean")
    return None


def install():
    """Install the project binaries to the system."""
    exec_mk("install")
    return None


def uninstall():
    """Uninstall the project binaries from the system."""
    exec_mk("uninstall")
    return None


def prepare_env():
    """Prepare the environment."""
    os.environ["CLR_YELLOW"] = "\"{}\"".format(CLR_YELLOW)
    os.environ["CLR_GRAY"] = "\"{}\"".format(CLR_GRAY)
    os.environ["CLR_RESET"] = "\"{}\"".format(CLR_RESET)
    os.environ["ROOT_DIR"] = ROOT_DIR
    os.environ["SRC_ROOT_DIR"] = SRC_ROOT_DIR
    os.environ["TMP_ROOT_DIR"] = TMP_ROOT_DIR
    os.environ["VERSION"] = VERSION
    os.environ["EXEC"] = EXEC
    return None


def cppcheck():
    """Run the Cppcheck on the source tree."""
    clear_screen()
    subprocess.call(
        ["cppcheck", "-q", "--enable=all", "--inconclusive", SRC_ROOT_DIR])
    return None


def memcheck(test):
    """
    Run the memory checks for the project executables.

    Parameters
    ----------
    test: bool
      If True, tests would be executed also.
    """

    exec_cmd = EXEC
    if test:
        exec_cmd += " -t"
    clear_screen()
    subprocess.call([
        "valgrind",
        "-v",
        "--leak-check=full",
        "--show-leak-kinds=all",
        "--track-origins=yes",
        "--leak-check=yes",
        "--num-callers=64",
        exec_cmd])
    return None


def clear_screen():
    """Clear the terminal screen."""
    subprocess.call(["reset"])
    return None


def main():
    """The entry point."""

    parser = argparse.ArgumentParser(description="Manage GSh")
    parser.add_argument("-c", "--clean", help="clean", action="store_true")
    parser.add_argument("-i", "--install", help="install", action="store_true")
    parser.add_argument(
        "-u", "--uninstall", help="uninstall", action="store_true")
    parser.add_argument("-b", "--build", help="build", action="store_true")
    parser.add_argument(
        "-g", "--gen_deps", help="generate dependencies", action="store_true")
    parser.add_argument(
        "-d", "--debug", help="build in debug mode", action="store_true")
    parser.add_argument(
        "-t", "--test", help="build with tests", action="store_true")
    parser.add_argument(
        "-p", "--bench", help="build with benchmarks", action="store_true")
    parser.add_argument(
        "-k", "--cppcheck", help="run cppcheck on the source code", action="store_true")
    parser.add_argument(
        "-m", "--memcheck", help="run memcheck", action="store_true")
    args = parser.parse_args()

    prepare_env()

    if (args.build):
        builder = Builder(args.debug, args.test, args.bench)
        builder.build()

    if (args.gen_deps):
        build = Builder(args.debug, args.test, args.bench)
        build.gen_deps()

    if (args.clean):
        clean()
        return None

    if (args.install):
        install()
        return None

    if (args.uninstall):
        uninstall()
        return None

    if (args.cppcheck):
        cppcheck()
        return None

    if (args.memcheck):
        memcheck(args.test)
        return None

    return None

if __name__ == "__main__":
    main()
