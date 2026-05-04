
# start(<component_name>)
#
# main(<component_name>, <component_bias>)
# <component_name> must be the class name
# <component_bias> must be the bias relative to the number of ops
#
# end(<component_name>)

from core.test_conf import TEST_CONF

TEST_CONF.SYS_EMU_MAX_CYCLES = 100000000000

BASE_DIR = os.path.dirname(os.path.realpath(__file__))
exec(open(f"{BASE_DIR}/conf/regress/rand.py").read())

main("PowerSequence", 100)
