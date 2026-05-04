<%
   num_lhid_map = len(lhid_map)
   act_hids = str(lhid_map)[1:-1]
%>
int16_t __attribute__((section ("mtg_LHID_MAP_REGION"))) lhid_mapping[${num_lhid_map}] = {${act_hids}};
