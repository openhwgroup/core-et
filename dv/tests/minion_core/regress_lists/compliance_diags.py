# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

compliance_test_dir = COMPLIANCE_TESTS

compliance_tests = {
    'rv32mi': ['breakpoint', 'csr', 'illegal', 'ma_addr', 'ma_fetch', 'mcsr', 'sbreak', 'scall', 'shamt'],
    'rv32si': ['csr', 'dirty', 'ma_fetch', 'sbreak', 'scall', 'wfi'],
    'rv32ua': ['amoadd_w', 'amoand_w', 'amomax_w', 'amomaxu_w', 'amomin_w', 'amominu_w', 'amoor_w', 'amoswap_w', 'amoxor_w', 'lrsc'],
    'rv32uc': ['rvc'],
    'rv32ud': ['fadd', 'fclass', 'fcmp', 'fcvt', 'fcvt_w', 'fdiv', 'fmadd', 'fmin', 'ldst', 'recoding'],
    'rv32uf': ['fadd', 'fclass', 'fcmp', 'fcvt', 'fcvt_w', 'fdiv', 'fmadd', 'fmin', 'ldst', 'move', 'recoding'],
    'rv32ui': ['add', 'addi', 'and', 'andi', 'auipc', 'beq', 'bge', 'bgeu', 'blt', 'bltu', 'bne', 'fence_i', 'jal', 'jalr', 'lb', 'lbu', 'lh', 'lhu', 'lui', 'lw', 'or', 'ori', 'sb', 'sh', 'simple', 'sll', 'slli', 'slt', 'slti', 'sltiu', 'sltu', 'sra', 'srai', 'srl', 'srli', 'sub', 'sw', 'xor', 'xori'],
    'rv32um': ['div', 'divu', 'mul', 'mulh', 'mulhsu', 'mulhu', 'rem', 'remu'],
    'rv64mi': ['access', 'breakpoint', 'csr', 'illegal', 'ma_addr', 'ma_fetch', 'mcsr', 'sbreak', 'scall'],
    'rv64si': ['csr', 'dirty', 'ma_fetch', 'sbreak', 'scall', 'wfi'],
    'rv64ua': ['amoadd_d', 'amoadd_w', 'amoand_d', 'amoand_w', 'amomax_d', 'amomax_w', 'amomaxu_d', 'amomaxu_w', 'amomin_d', 'amomin_w', 'amominu_d', 'amominu_w', 'amoor_d', 'amoor_w', 'amoswap_d', 'amoswap_w', 'amoxor_d', 'amoxor_w', 'lrsc'],
    'rv64uc': ['rvc'],
    'rv64ud': ['fadd', 'fclass', 'fcmp', 'fcvt', 'fcvt_w', 'fdiv', 'fmadd', 'fmin', 'ldst', 'move', 'recoding', 'structural'],
    'rv64uf': ['fadd', 'fclass', 'fcmp', 'fcvt', 'fcvt_w', 'fdiv', 'fmadd', 'fmin', 'ldst', 'move', 'recoding'],
    'rv64ui': ['add', 'addi', 'addiw', 'addw', 'and', 'andi', 'auipc', 'beq', 'bge', 'bgeu', 'blt', 'bltu', 'bne', 'fence_i', 'jal', 'jalr', 'lb', 'lbu', 'ld', 'lh', 'lhu', 'lui', 'lw', 'lwu', 'or', 'ori', 'sb', 'sd', 'sh', 'simple', 'sll', 'slli', 'slliw', 'sllw', 'slt', 'slti', 'sltiu', 'sltu', 'sra', 'srai', 'sraiw', 'sraw', 'srl', 'srli', 'srliw', 'srlw', 'sub', 'subw', 'sw', 'xor', 'xori'],
    'rv64um': ['div', 'divu', 'divuw', 'divw', 'mul', 'mulh', 'mulhsu', 'mulhu', 'mulw', 'rem', 'remu', 'remuw', 'remw'],
}

p_only = {'rv32mi', 'rv32si', 'rv64mi', 'rv64si'}

for subdir, names in compliance_tests.items():
    envs = ['p'] if subdir in p_only else ['p', 'v']
    for env in envs:
        for name in names:
            t(f'{subdir}-{env}-{name}', f'compliance/{subdir}', ['neigh'], tags=['minion_diags', 'compliance'])
