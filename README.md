# IO_Speed Test
test repo for page cache I/O, Direct I/O and mmap


## prerequisite

g++

## Get Started


### 

use following command to build up test
```
make build
```

then run test using 

```
./main
```

it will generate some test files(4MB) under directory `./tmp`, read/write 100 times for each I/O mode and print time usage on command line.

## clean up

use `make clean` to clean up all garbage from test
