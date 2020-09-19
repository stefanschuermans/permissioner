# Permissioner: Setting File Ownerships and Permissions

The unix tools `chown` and `chmod` allow to set ownership and permissions of
files and entire directory trees. However, multiple calls to those tools
are needed when setting complex ownerships and permissions of nested directory
trees. In case the change of ownerships and permissions shall be very fast for
file, e.g., because the directory tree is accessed in parallel to the change,
using multiple calls is not suitable.

For example, let's assume, multiple users write files to a shared directory
`shared/` in order to implement a primitive ad-hoc file sharing. The users
all have the group `fileshare`. All files copied to the shared directory
shall be readable, writable and deletable by all other users in the group.
However, there is one directory, called `shared/perm`, in which files should
stay permanently and be only readable for all users. This could be implemented
by executing the following commands periodically in a `cron`-job:

```
chown -R nobody:fileshare shared
chmod -R ug+rwX shared
chmod -R g-w shared/perm
```

However, while the sequence of the three commands is executing, strange and
unwanted ownerships and permissions may occur. If the directory tree is large,
it can lead to problems accessing the files in parallel.

Permissioner can help to reduce the problems by touching each file only once,
and setting its ownership and permissions very quickly, before advancing to the
next file.

Using the configuration file `fileshare.cfg` with the content

```
tree nobody fileshare ug+rwX shared
tree nobody fileshare u+rwX,g+wX,g-w shared/perm
```

in the call

```
bin/permissionerc fileshare.cfg
```

has the same effect, but achieves the same outcome while touching every file
just once - thus avoiding the transient strange state of files.

This is only a simple example to illustrate the functionality of
permissioner. There are various other tools to properly implement a file
sharing service. The sketched setup is only an ad-hoc hack and not a proper
solution.

Situations in which `chown` and `chmod` are not sufficient are no very common.
Thus, permissioner is a very specific tool for a very specific use case. If
you are not sure if you should use permissioner, after reading the above
example, you should probably stick with the Unix tools `chown` and `chmod`.

## Building

Permissioner is developed on Debian Linux 10 "buster".

Install the dependencies:

```
apt-get install -y build-essential cmake gcc g++ ninja-build \
```

Change to the directory of this `REAMDE.md` file.

Configure a build directory:

```
mkdir build
cd build
cmake -G Ninja -D CMAKE_BUILD_TYPE=Release ..
```

Build:

```
ninja
```

Run tests:

```
ctest
```

## Config File

The configuration file lists directory trees and the ownerships and permissions
to set for them. If some of the specified trees are nested within each other,
the nested tree(s) is/are excluded from the containing tree(s).

The syntax of the config file is line-based. Each line defines a setting or a
directory tree and the ownerships and permissions for it.

Syntax:

   * process nice-ness (only for `permissionerd`, ignored by `permissonerc`)
     `nice [<niceness value>] [idle]
      * `<niceness value>`: Linux niceness value for daemon process,
                            `19` for nicest (lowest CPU priority)
      * `idle`: set I/O priority class to idle
   * sleep time after each file: `sleepTime <seconds>`
      * range 0 - 1 s, default 1e-6 s
   * wait time factor: `waitFactor <floating-point factor>`
      * how long to wait after each tree traversal relative to the duration of
        the traversal
      * range 0 - 1000, default 10
   * additional wait time after each tree traversal: `waitTime <seconds>`
      * range 0 - 3600 s, default 1 s
   * owership and permission configuration:
     `tree <user> <group> <permissions> <directory>`
      * `<user>`: User name to set as user/owner, `-` to not change the user/owner.
      * `<group>`: Group name to set as group, `-` to not change the group.
      * `<permissions>`: Comma-separated list of permission settings.
         * `<perm setting>[,<perm setting>[,<...>]]`
      * `<perm setting>`: Setting (`=`), adding (`+`) or removing (`-`) permissions.
         * `<who>[=+-]<what>`
      * `<who>`: For whom to change the permissions. Any combination of:
         * `u`: User.
         * `g`: Group.
         * `o`: Others.
      * `<what>`: Which permissions to change.
         * `r`: Reading.
         * `w`: Writing.
         * `x`: Executing for files, browsing for directories.
         * `X`: Like `x` if `x` set for user/owner of the file.
      * `<directory>`: Absolute or relative directory name.
         * Defines the base of a directory tree to process.
         * Relative directory names are relative to the woking directory of
           `permissionerc` or `permissionerd`.

Example:

```
tree nobody fileshare ug+rwX shared
tree nobody fileshare u+rwX,g+wX,g-w shared/perm
```

## Permissioner Client

The permissioner client is a simple binary that reads the config file and
sets the ownerships and permissions according to the directory tress configured
in it once.

Call syntax:

```
bin/permissionerc <config file>
```

## Permissioner Daemon

The permissioner daemon set ownerships and permissions repeatedly.

Call syntax:

```
bin/permissionerd <config file>
```

The daemon will execute the following actions in a loop:

* Set ownwerships and permissions according to the config file,
  while measuring the time.
* Wait (sleep) for 10 times as long as it took plus one second.
* Repeat.

The daemon continues to run until it receives an interrupt (`SIGINT`) or
termination (`SIGTERM`) signal.
