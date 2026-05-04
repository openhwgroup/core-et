#!/usr/bin/python3
# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0


import csv

# Expected header
EXPECTED_HEADER = [
    "Mod", "PP", "Address", "Name", "Access", "Description", "Type", "width",
    "rdata", "wdata", "shadow", "resetVal", "reset", "triggerCnt", "latch",
    "connect_to_port", "implemented", "ifdef", "comment"
]

def parse_csv(file_path):
    """
    Parse the CSV file and return a list of dictionaries.
    Each dictionary corresponds to a row in the CSV.
    """
    parsed_rows = []
    with open(file_path, mode="r", newline="", encoding="utf-8") as csvfile:
        reader = csv.DictReader(csvfile)
        
        # Validate header
        if reader.fieldnames != EXPECTED_HEADER:
            raise ValueError(f"CSV header does not match expected header.\n"
                             f"Found: {reader.fieldnames}\n"
                             f"Expected: {EXPECTED_HEADER}")
        
        # Parse rows
        for row in reader:
            parsed_rows.append(row)
    
    return parsed_rows

def format_hex(value):
    hex_str = hex(value)
    prefix = "0x"
    digits = hex_str[len(prefix):]

    # Reverse the digits to group from the right
    reversed_digits = digits[::-1]

    # Group into chunks of 4
    grouped_reversed = "_".join(
        reversed_digits[i:i+4] for i in range(0, len(reversed_digits), 4)
    )

    # Reverse back to original order
    grouped = grouped_reversed[::-1]

    return prefix + grouped

def main():
    file_path = "esr.csv"  # Replace with your CSV file path

    esr_base_addr = 0x100000000

    module_addr = dict()
    module_addr['neigh'] = 0x100000
    module_addr['shire_other'] = 0x340000

    pp_offset = 30

    try:
        data = parse_csv(file_path)
        print(f"Parsed {len(data)} rows successfully.")

        filt_data = filter(lambda x: x['Mod'] in module_addr.keys() and x['implemented'] == 'YES', data)       
 
        for i, row in enumerate(filt_data, start=1):

            if not row["Name"] in ["tbox_rbox_dbg_rc", "texture_control", "texture_status", "texture_image_table_ptr"]:
                mod = row["Mod"]
                addr = row["Address"]

                reg_offset = int(addr, 16) << 3
                mod_offset = module_addr[mod]

                base_mem_addr = mod_offset + reg_offset
                #print(hex(base_mem_addr))

                #                                 ESR REGION                PP_OFFSET
                et_mem_addr     = base_mem_addr + 0x100000000 + (int(row["PP"]) << 30)
                erbium_mem_addr = base_mem_addr +  0x80000000 + (int(row["PP"]) << 22)

                #print(f"ET:     {row['Name']} = {hex(et_mem_addr)}")
                #print(f"Erbium: {row['Name']} = {hex(erbium_mem_addr)}")
                print(f"{row['Name']};{format_hex(et_mem_addr)};{format_hex(erbium_mem_addr)}")
    
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    main()
