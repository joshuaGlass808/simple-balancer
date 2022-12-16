#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <bpf/bpf_helpers.h>
#include <yaml.h>

struct bpf_map_def SEC("maps")
nodes_map = {
  .type = BPF_map_TYPE_HASH,
  .key_size = sizeof(u32),
  .value_size = sizeof(u32),
  .max_entries = 2,
};

int parse_yaml(char *yaml_str)
{
  yaml_parser_t parser;
  yaml_node_t *node;
  if (!yaml_parser_initialize(&parser)) return -1;

  yaml_parser_set_input_string(&parser, yaml_str, strlen(yaml_str));
  if (!yaml_parser_load(&parser, &node)) return -1;
  
  yaml_node_t *nodes_node = yaml_document_get_node(node, (yaml_char_t*)"nodes");
  if (!nodes_node) return -1;

  for (int i = 0; i < nodes_node->data.sequence.items.top; i++) {
    yaml_node_t *node_node = nodes_node->data.sequence.items.start[i];
    yaml_node_t *address_node = yaml_document_get_node(node, (yaml_char_t*)"address");
    if (!address_node) continue;
    u32 key = i;
    u32 val;
    sscanf(address_node->data.scalar.value, "%u", &val);
    bpf_map_update_elem(&nodes_map, &key, &val, BPF_ANY);
  }

  yaml_parser_delete(&parser);

  return 0;
}

SEC("xdp_sock")
int xdp_sock_prog(struct xdp_md *ctx)
{
  void *data_end = (void *)(long)ctx->data_end;
  void *data = (void *)(long)ctx->data;
  struct ethhdr *eth = data;

  if (eth + 1 > data_end) return XDP_PASS;
  if (eth->h_proto != htons(ETH_P_IP)) return XDP_PASS;

  struct iphdr *iph = data + sizeof(*eth);
  if (iph + 1 > data_end) return XDP_PASS;

  u32 key = 0;
  u32 *val;
  bpf_map_lookup_elem(&nodes_map, &key, &val);
  *val += 1;

  if (*val % 2 == 0) {
    // send traffic to first
    bpf_printk("Going to first");
  } else {
    // send traffice to second
    bpf_printf("Going to second");
  }

  return XDP_PASS;

}

int main(int argc, char **argv)
{
  if (argc != 2) return -1;

  char *yaml_str = argv[1];
  if (parser_yaml(yaml_str) < 0) return -1;

  return 0;
}

char _license[] SEC("license") = "GPL";
