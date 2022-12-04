#include "xdp_lb_kern.h"

#define IP_ADDRESS(x) (unsigned int)(172 + (17 << 8) + (0 << 16) + (x << 24))

#define BACKEND_A 2
#define BACKEND_B 3
#define CLIENT 4
#define LB 5

SEC("xdp")
int xdp_load_balancer(struct xdp_md *ctx)
{
    // void *data = (void *)(long)ctx->data;
    // void *data_end = (void *)(long)ctx->data_end;

    bpf_printk("got something");

    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";
