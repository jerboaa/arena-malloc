# Test glibc's arena allocations in containers

This is a project with a simple multi-threaded C program exercising malloc.
It's useful to empirially verifying glibc's `malloc_arena_new` calls.

## Usage

First, build 32 bit and 64 bit containers:

    $ ./build_docker.sh

Next, run the containers as follows:

    $ sudo docker run -it --cap-add=SYS_PTRACE --rm jerboaa/arena-malloc:1-centos7-64bit | grep -E 'TOTAL|processors|Spawning'
    This system has 8 processors configured and 8 processors available.
    Spawning a total of 72 (> (8 * 8)) threads
    $64 = "TOTAL memory_arena_new calls: 63" 

    $ sudo docker run -it --cap-add=SYS_PTRACE --rm jerboaa/arena-malloc:1-centos7-32bit | grep -E 'TOTAL|processors|Spawning'
    This system has 8 processors configured and 8 processors available.
    Spawning a total of 24 (> (2 * 8)) threads
    $16 = "TOTAL memory_arena_new calls: 15" 

The expectation is that it should be bound above by `8 * # CPU cores` on 64 bit
and `2 * # CPU cores` on 32 bit.

## Running with CPU Quota

Assigning only ~2 CPU cores to the container:

    $ sudo docker run -it --cpu-period="100000" --cpu-quota="200000" --cap-add=SYS_PTRACE --rm jerboaa/arena-malloc:1-centos7-64bit | grep -E 'TOTAL|processors|Spawning'
