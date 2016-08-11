# dhtpim 0.0.1

Simple OpenDHT private instant messaging.

```
Usage: ./dhtpim -u <username> -k <keychain> [-p <password>]

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

# Build

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
    ./dhtpim -u anon -c keychan -p secret
```
# Note on privacy

As long you are using a strong password, dhtpim does provide some kind of
privacy, but note that nothing is done to hide metadata.
