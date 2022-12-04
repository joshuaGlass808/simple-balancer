# simple-balancer
First attempt at a simple balancer with eBPF. Going to try out gobpf at first. https://kinvolk.io/blog/2016/11/introducing-gobpf-using-ebpf-from-go/

But may end up just using libbpf directly.



Using libbpf

git clone this repo on a linux server (ubuntu)
```
sudo docker build .
sudo docker create <container_id> 
sudo docker commit <container> ubuntu-working
sudo docker run --rm -it -v ~/simple-balancer:/simple-balancer --privileged -h lb --name lb --env TERM=xterm-color ubuntu-working
```
Once in the container run:
`cd simple-balancer/bpftool/src && make install && cd ../.. && make`

From here, open up another tab and on the linux host you are running the LB, run `sudo cat /sys/kernel/debug/tracing/trace_pipe` to see the print statements
