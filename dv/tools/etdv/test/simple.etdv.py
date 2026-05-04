# Copyright (c) 2026 Ainekko
# SPDX-License-Identifier: Apache-2.0

builds = config._builds
config_name = 'my_config'
#env = ['MYENV', 'YOYO']
build_cmd = """
cat <<L1 > ${ETDV_RUNDIR}/runme
#!/bin/csh -f
echo Error:here
sleep \$1
exit 0
L1
chmod +x ${ETDV_RUNDIR}/runme
"""
#et-dvrun-check-log build|test log_file status_file die_on_fail
check_cmd='echo pass > $TARGET'
#check_cmd='et-dvrun-check-log build $LOG $TARGET 0'

build = new_build(name='my_buildA',run_cmd=build_cmd,check_cmd=check_cmd)
build['slurmopts'] = '-slurmOpts1=val -L mylic1:1,mylic2'
#build['test_slurmopts'] = 'test-slurm-opt1 ts-opts2'
build['test_slurmopts'] = '-L lic3 --time=56'
add_build(build)
for i in [2,5,10]:
    run_cmd = f'${{ETDV_BUILD_RUNDIR}}/runme {i}'
    test = new_test(name=f'my_test_{i}', run_cmd=run_cmd,check_cmd=check_cmd,tags=['smoke','random'])
    #test['slurmopts'] = f"{build['slurmopts']} --time=cfg66666666"
    test['slurmopts'] = '-L lic2 --time=34'
    build.add_test(test)

build.add_test(
    new_test(
        name='test_nightly',
        run_cmd='${ETDV_BUILD_RUNDIR}/runme 3',
        check_cmd=check_cmd,
        tags=['nightly']*3
    )
)
build = new_build(name='my_buildB',run_cmd=build_cmd,check_cmd=check_cmd)
add_build(build)
for i in [2,5,10]:
    run_cmd = 'echo hello\n' + f'${{ETDV_BUILD_RUNDIR}}/runme {i}\n' + 'echo world\n'
    test = new_test(name=f'my_test_{i}', run_cmd=run_cmd,check_cmd=check_cmd,tags=['smoke','random'])
    build.add_test(test)

build.add_test(
    new_test(
        name='test_nightly',
        run_cmd='${ETDV_BUILD_RUNDIR}/runme 3',
        check_cmd=check_cmd,
        tags='nightly',
        slurmopts='-testSlurmOpts1=val'
    )
)

uarg = get_user_arg()

print(f'etdv_random_seed()={etdv_random_seed()}')
print(f'etdv_random()={etdv_random()}')
print(f'etdv_random()={etdv_random()}')
print(f'etdv_random()={etdv_random()}')
print(f'etdv_randint(4,600)={etdv_randint(4,600)}')

from ETdv.dvrun.main import Main
opts = Main.the_one().options
print(f"opts is type:{type(opts)} and has [] operator: {hasattr(opts, '__getitem__')}")
hasattr(opts, '__getitem__')
[print(f'{k}={opts[k]}') for k in ['--slurmopts','--config']]

if Main.the_one().has_option('--tags') and 'b4c_stage1' in Main.the_one().options['--tags']:
  print('yes')

if False:
	print(f"before PATH={getenv('PATH')}")
	import os
	os.environ['PATH'] = getenv('PATH') + ":morepathstuff:here"
	print(f"after PATH={getenv('PATH')}")
