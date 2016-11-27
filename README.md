## how to use
the program can be used to dump the information of the MBR/EBR，or can generate a new partition table.

### dump the MBR/EBR
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

### generate a new partition table
**this can be dangerous**, this can only **extend the third partition with the fourth partition**, to use:

```c
$ make modify
# ./parted /dev/sda
```

after this, a file named "result" is generated, to apply, run the following command(**before run, plz be aware what you're doing; if not, stop here**):

```c
# dd if=result of=/dev/sda bs=512 count=1
```


