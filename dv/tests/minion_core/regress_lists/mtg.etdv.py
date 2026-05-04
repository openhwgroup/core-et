# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0



mtg_num_ops_mult  = 1
mtg_num_tests     = 1
mtg_num_test_iters= 1
mtg_plusargs = '+en_vpu_scp_read_conflict_tload_scp_chk=0 +en_tstore_scp_conflict_tload_scp_chk=0'

mtg_user_args = [arg.split('=')[-1] for arg in usr_args if arg.startswith('mtg_args=')]
if len(mtg_user_args) > 0:
   assert len(mtg_user_args) == 1, "Multiple 'mtg_args' detected. Found {mtg_user_args}"
   mtg_user_args = mtg_user_args[0]
   info('Msgs-1', f'MTG user arguments are: {mtg_user_args}')
else:
   mtg_user_args = ""

if usr_args.count('postsi_ust') != 0 or usr_args.count('postsi') != 0:
   if not any(mtg_arg in mtg_user_args for mtg_arg in ["-ps", "--post_silicon"]):
      mtg_user_args += " -ps "

if any(mtg_arg in mtg_user_args for mtg_arg in ["-ma", "--minion_agnostic", "-na", "--neigh_agnostic", "-sa", "--shire_agnostic"]):
   mtg_sweep_mode = True
else:
   mtg_sweep_mode = False

# MTG NUM_OPS multiplier
mtg_mult_opt = [arg.split('=')[-1] for arg in usr_args if arg.startswith('mtg_num_ops_mult=')]
if mtg_mult_opt:
   mtg_num_ops_mult = float(mtg_mult_opt[0])
   info('Msgs-1', f'MTG multiplier for number of operations: {mtg_num_ops_mult}')

# MTG version
MTG_ROOT = f"{os.environ['REPOROOT']}/extern/mtg"
assert os.path.isdir(MTG_ROOT), 'MTG directory is empty'
setenv('MTG_ROOT', MTG_ROOT)
mtg_version = os.popen(f"cd {MTG_ROOT} && git rev-parse --short HEAD").read()
info('Msgs-1', f'Using MTG version {mtg_version}')

def add_build_sysemu(run_cmd):
   run_cmd += 'mkdir -p $REPOROOT/emu/bemu/sys_emu/build\n'
   run_cmd += f'[[ -f $REPOROOT/emu/bemu/sys_emu/build/sys_emu ]] || flock $REPOROOT/emu/bemu/sys_emu/build make -C $REPOROOT/emu/bemu/sys_emu &&\n'
   return run_cmd


def compile_mtg_test(run_cmd, mtg_conf, mtg_args, seed, test_plusargs):
   run_cmd += f'export MTG_ROOT={MTG_ROOT}\n'
   run_cmd += f'export MTG_TEST_PWD=$(pwd)\n'
   run_cmd += "export POSTSI_CFG_RESUME_DRET=1\n"

   # Build sys_emu binary
   #run_cmd = add_build_sysemu(run_cmd)

   #elf_name = os.path.splitext(mtg_conf)[0]
   elf_name = 'base'

   shires  = re.search("\+SHIRE_MASK=\S+", test_plusargs)
   minions = re.search("\+MINION_MASK=\S+", test_plusargs)
   threads = re.search("\+THREAD_MASK=\S+", test_plusargs)
   shire_mask  = shires.group().split('=', 1)[-1]  if shires  is not None else '0x1'
   minion_mask = minions.group().split('=', 1)[-1] if minions is not None else '0x1'
   thread_mask = threads.group().split('=', 1)[-1] if threads is not None else '0x1'

   run_cmd += "mkdir -p build &&\n"
   run_cmd += f'$MTG_ROOT/mtg.py -thr {thread_mask} -min {minion_mask} -shi {shire_mask} -c {mtg_conf} -s {seed} --self_check 0 {mtg_args} build &&\n'
   return run_cmd

def postcompile_mtg_test(run_cmd, elf_name="base"):
   ## The next three lines are a hack to workaround an error in the post_si diag flow. One of the scripts expects there to be a test.elf soft link pointing to a lower level of hierearchy
   run_cmd += "mkdir -p test_elf\n"
   run_cmd += f'cp {elf_name} test_elf/\n'
   run_cmd += f'ln -sfn test_elf/{elf_name} test.elf &&\n'
   run_cmd += f'elf2hex {elf_name} memImage.hex && echo "ELF Load: {elf_name}" > mem_desc.txt &&\n'
   return run_cmd


def add_mtg_test(build, test_name, mtg_args='', test_plusargs='', test_tags=[], checker=True, mtg_conf=f'rand.py', board_conf='1c', mtg_conf_vars=[]):
   build_tags = ['random', 'mtg']
   build_tags.extend(test_tags)
   seed = etdv_randint() if user_seed is None else user_seed

   add_diag(build, test_name, '', test_plusargs=test_plusargs, test_tags=build_tags, ddr_code_preload=False, mtg_test=True, mtg_conf=mtg_conf, mtg_args=mtg_args)



#-------------------------------------------------- MTG configurations -------------------------------------------------
mtg_builds = {
   '1t': {
      'threads'            : 1,
      'minions'            : 1,
      'build'              : 'neigh',
      'seeds'              : 10,
      'ops_x'              : 1,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x1 +MINION_MASK=0x1 +START_PC=40000000 ',
      'base_minion_mask'   : 0x1,
      'mtg_args'           : ''
   },
   '2t': {
      'threads'            : 2,
      'minions'            : 1,
      'build'              : 'neigh',
      'seeds'              : 10,
      'ops_x'              : 1,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0x1 +START_PC=40000000 ',
      'base_minion_mask'   : 0x1,
      'mtg_args'           : ''
   },
   '4t': {
      'threads'            : 4,
      'minions'            : 2,
      'build'              : 'neigh',
      'seeds'              : 10,
      'ops_x'              : 1,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0x3 +START_PC=40000000 ',
      'base_minion_mask'   : 0x3,
      'mtg_args'           : ''
   },
   '16t': {
      'threads'            : 16,
      'minions'            : 8,
      'build'              : 'neigh',
      'seeds'              : 10,
      'ops_x'              : 1,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0xff +START_PC=40000000 ',
      'base_minion_mask'   : 0xff,
      'mtg_args'           : ''
   },
   '1s4n4t': {
      'threads'            : 16,
      'minions'            : 8,
      'build'              : '1s4n4t',
      'seeds'              : 5,
      'ops_x'              : 1,
      'plusargs'           : ' +thread_timeout=350000 +trans_timeout=350000 +THREAD_MASK=0x3 +MINION_MASK=0x03030303 ',
      'base_minion_mask'   : 0x03030303,
      'mtg_args'           : ''
   },
   '64t': {
      'threads'            : 64,
      'minions'            : 32,
      'build'              : 'shire',
      'seeds'              : 10,
      'ops_x'              : 0.5,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff ',
      'base_minion_mask'   : 0xffffffff,
      'mtg_args'           : ''
   },
   '128t': {
      'threads'            : 128,
      'minions'            : 64,
      'build'              : 'shirex2',
      'seeds'              : 10,
      'ops_x'              : 0.5,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0x3 ',
      'base_minion_mask'   : 0xffffffff,
      'mtg_args'           : ''
   },
   '256t': {
      'threads'            : 256,
      'minions'            : 128,
      'build'              : 'shirex4',
      'seeds'              : 10,
      'ops_x'              : 0.5,
      'plusargs'           : ' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK=0xffffffff +SHIRE_MASK=0xf ',
      'base_minion_mask'   : 0xffffffff,
      'mtg_args'           : ''
   },
}

vcs_builds = {
   64  : 'shire',
   128 : 'shirex2',
   192 : 'shirex4',
   256 : 'shirex4'
}

threads = list(range(64, 64*35, 64))
for th in threads:
   # skip builds that have specific requires
   if f"{th}t" in mtg_builds:
      continue

   try:
      vcs_build = vcs_builds[th]
   except KeyError:
      vcs_build = 'fc'

   min_mask = 0xffffffff
   shi_mask = int(2**(th/64)-1)
   build = {
      'threads'            : th,
      'minions'            : int(th/2),
      'build'              : vcs_build,
      'seeds'              : 2,
      'ops_x'              : 1,
      'plusargs'           : f' +thread_timeout=100000 +trans_timeout=40000 +THREAD_MASK=0x3 +MINION_MASK={hex(min_mask)} +SHIRE_MASK={hex(shi_mask)} ',
      'base_minion_mask'   : min_mask,
      'mtg_args'           : ''
   }
   mtg_builds[f"{th}t"] = build

build_maxharts = { 'neigh':   16,
                   '1s1n16t': 16,
                   '1s4n4t':  16,
                   'shire':   64,
                   'shirex2': 128,
                   'shirex4': 256,
                   'shirex8': 512,
                   'fc':      2048
                 }



#-----------------------------------------------------------------------------------------------------------------------

mtg_plusargs += ' +WFI_ENDS_TEST=0 +ECALL_ENDS_TEST=0 +clk_gate_random '
mtg_common_args = ''

comp_dir      = MTG_ROOT + '/conf/regress'
excluded_confs = ['base.py']

MTGTestTemplate = namedtuple('MTGTestTemplate', [
   'build',
   'test_name',
   'board_conf',
   'test_tags',
   'test_plusargs',
   'mtg_args',
   'mtg_conf',
   'mtg_conf_vars'
])
mtg_test_templates = []

# Go through both the repo components
for root_dir in [comp_dir]:
   for dir, sub_dir, files in os.walk(root_dir):
      for f in [f for f in files if (f.endswith('.py') and not f in excluded_confs)]:

         conf_plusargs   = ''
         base_tag        = 'mtg'
         conf_name       = os.path.splitext(f)[0]
         test_multiplier = 1
         if mtg_sweep_mode:
            conf_num_ops    = 512
         else:
            conf_num_ops    = 2500
         min_harts       = 0
         max_harts       = 2*8*4*34
         skip_build      = ''
         et_dvrun_tags   = ['mtg_regress']
         mtg_conf_vars = []

         # For each model add as many seeds as desired. Or skip it if there are not enough HARTs
         for b in mtg_builds:
            num_harts = mtg_builds[b]['threads']
            if num_harts < min_harts or num_harts > max_harts:
               continue
            num_seeds = mtg_builds[b]['seeds']
            num_seeds = int(num_seeds * test_multiplier)
            num_ops   = int(conf_num_ops * mtg_builds[b]['ops_x'] * mtg_num_ops_mult)
            conf_build = mtg_builds[b]['build']
            # Adding some logic here to randomize the HARTs the test is run on for the 1t, 2t and 16t builds
            if b in ['1t', '2t', '16t']:
               if ("+MINION_MASK" in f'{extra_sim_opts}'):
                  build_plusargs = mtg_builds[b]['plusargs']
               else:
                  #minion_shift = random.randrange((32 / mtg_builds[b]['minions']) - 1)
                  minion_shift = 0
                  minion_mask = f"+MINION_MASK=" + str(hex(mtg_builds[b]['base_minion_mask'] << (minion_shift * mtg_builds[b]['minions'])))
                  import re
                  build_plusargs = re.sub(r"\+MINION_MASK=0x[^ ]+", minion_mask, mtg_builds[b]['plusargs'])
            else:
               build_plusargs = mtg_builds[b]['plusargs']
            if not (conf_build == skip_build):
               # Add verilator build for Neigh tests
               cbuilds = ["neigh", "cpu_ss_vl", "cpu_ss_vcs"] if conf_build == "neigh" else [conf_build]

               for cb in cbuilds:
                  mtg_test_templates.append((num_seeds,
                     MTGTestTemplate(
                                    build         = builds[cb],
                                    test_name     = f'{base_tag}_{b}_{conf_name}',
                                    board_conf    = f'{num_harts}c',
                                    test_tags     = [base_tag, conf_name, f'{base_tag}_{b}', f'{base_tag}_{num_harts}t_{conf_name}'] + et_dvrun_tags,
                                    test_plusargs = (conf_plusargs + ' ' + mtg_plusargs + build_plusargs),
                                    mtg_args    = f'{mtg_builds[b]["mtg_args"]} -ncomps {num_ops} {mtg_user_args}',
                                    mtg_conf    = f,
                                    mtg_conf_vars = mtg_conf_vars
                                    )))

mtg_saved_test_dir = MINION_DIAGS + '/mtg'
saved_tests = list()
for root, dirs, files in os.walk(mtg_saved_test_dir, followlinks=True):
   for f in files:
      if f.endswith('.elf'):
         saved_tests.append((root, f))
for (r, t) in saved_tests:
   test_build = t.split("_")[0]
   if "jira" in r:
      tags = ['jira_mtg']
   elif "screen" in r:
      tags = ['si_screen']
   else:
      tags = ['saved_mtg']
   test_name = f"mtg_{t}".replace('.elf', '_elf')

   if test_build not in mtg_builds:
      info('Msgs-1', f'MTG: {test_build} build not found for the test {test_name}')
      continue

   build_plusargs = mtg_builds[test_build]['plusargs']
   add_diag(builds[mtg_builds[test_build]['build']], t, r, force_test_name=test_name, test_plusargs=mtg_plusargs+build_plusargs, test_tags=tags, compile_diag=False, mtg_test=True, ddr_code_preload=False)

if len(mtg_test_templates) > 0:
   max_num_seeds = max([num_seeds for (num_seeds, _) in mtg_test_templates])
   # Add tests in circular order, i.e.: foo_0, bar_0, baz_0, foo_1, bar_1, ...
   for ii in range(max_num_seeds):
      for (num_seeds, tmpl) in mtg_test_templates:
         if num_seeds > ii:
            add_mtg_test(build         = tmpl.build,
                         test_name     = tmpl.test_name + f'_{ii}',
                         board_conf    = tmpl.board_conf,
                         test_tags     = tmpl.test_tags,
                         test_plusargs = tmpl.test_plusargs,
                         mtg_args      = tmpl.mtg_args,
                         mtg_conf      = tmpl.mtg_conf,
                         mtg_conf_vars = tmpl.mtg_conf_vars)
