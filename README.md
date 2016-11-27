## how to use
to dump the mbr of the /dev/sda:

```c
$ make
# ./parted /dev/sda
```

because of the similarity of mbr and ebr, this can also be used to parse the ebr, i.e.:

```c
$ make
# ./parted /dev/sda4
```
