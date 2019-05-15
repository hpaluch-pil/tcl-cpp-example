# Embedding Tcl into C/C++ program

Here is trivial example how to embedd Tcl into your C/C++ program.

Thre are implemented 2 custom commands available from TCL:

1. Command `::ex::uname_machine` that returns
   machine name from `uname(2)` system call.

1. Command `::ex::uptime_seconds` that returns
   number of seconds from system start (uptime) from
   sysinfo(2)` system call.

Pleae see also [Embedding LUA into C/C++ program](https://github.com/hpaluch-pil/lua-cpp-example) for comparison.

> WARNING!
>
> Valgrind report lot of leaks but ... the threaded version
> uses pool of per-thread allocated memory - so it is much
> harder to interpret results...


# Setup

Tested OS: `Debian9.9 (Stretch)/amd64`

Install following required packages:

```bash
sudo apt-get install build-essential tcl tcl-dev tcl-doc libtcl8.6-dbg \
     git valgrind
```

Optional: Install Eclipse with CDT (C/C++ development support):

```bash
sudo apt-get install eclipse-cdt
```

TODO: import to Eclipse

# Building example

Just invoke:

```bash
make
```

# Running example

To run example just invoke:

```bash
make run
```

Following examples are run:

```tcl
puts "Hello, world on [::ex::uname_machine]!"
puts "System uptime is [::ex::uptime_seconds] seconds."
```

should produce output like:

```
Hello, world on x86_64!
System uptime is 12678 seconds.
```


To run example with `valgrind(1)` memory leak detector use:

```bash
make valgrind
```

> NOTE: See notice at the beginning of this readme...

# Resources

* official Tcl embedding docs:
  - https://wiki.tcl-lang.org/page/How+to+embed+Tcl+in+C+applications
* nice "Tcl crash course" from OpenOCD author:
  - http://openocd.org/doc/html/Tcl-Crash-Course.html


