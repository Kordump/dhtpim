# dhtpim 0.0.1

Simple OpenDHT private instant messaging.
```
Usage: ./dhtpim -u <username> -c <keychain> [-p <password>]

  -h, --help                   Print help and exit
  -V, --version                Print version and exit
  -v, --verbose                Verbose mode  (default=off)
  -i, --interactive            Interactive mode  (default=off)
  -u, --username=STRING        Username
  -c, --keychain=STRING        Channel to join
  -p, --password=STRING        Channel password
  -l, --port=INT               Force local node port  (default=`0')
  -r, --bootstrap-port=INT     Remote bootstrap port  (default=`4222')
  -b, --bootstrap-host=STRING  Remote bootstrap node
                                 (default=`bootstrap.ring.cx')
```

# Quick setup

Check if `opendht 0.6.3` and `gengetopt` are installed on your machine.

For instance, on archlinux :
```
    yaourt -a opendht
    # - edit the PKGBUILD
    # - replace 0.6.x by 0.6.3 (line 3)
    # - comment patch (line 21)

    yaourt -S gengetopt
```

Then, use the Makefile as usual :
```
    make
    cd bin
    ./dhtpim -u Nickname -c Channel -p Password
```

# IRC support

You can start a minimal IRC wrapper with `irc.sh` on your local machine.

For exemple :
```/bin/bash
    cd bin
    ./irc.sh Nickname Channel Password 2> /dev/null &
    irssi -c localhost
    killall irc.sh
```

Note :
 - This wrapper is not suitable for public usage.
 - The command `unbuffer` is owned by `expect` package.

# Note on privacy

As long as you are using a strong password, dhtpim does provide some kind of
privacy. However, please note that dhtpim does not provide and will not provide
any of the following guarantees :
 - Perfect Forward Secrecy.
 - Perfect Anonymity.
 - No Metadata leaks.

Enjoy !

