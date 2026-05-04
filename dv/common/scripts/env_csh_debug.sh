bash -c 'source env.sh || exit; for  i in $( compgen -e ); do echo setenv $i \"${!i}\"\; ; done'
