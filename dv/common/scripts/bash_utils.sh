#
# Simple Utilities for bash scripts 
#

#
# A couple of basic aliases to preprend and append items to $PATH without duplicates or sorting.
#
prepend_path() {
    # prepend
    PATH="$*${PATH:+":$PATH"}"
    
    #and remove duplicates
    export PATH=$(perl -e 'print join(":", grep { not $seen{$_}++ } split(/:/, $ENV{PATH}))')
}

append_path() {
    # append
    PATH="${PATH:+"$*:"}$ARG"
    
    #and remove duplicates
    export PATH=$(perl -e 'print join(":", grep { not $seen{$_}++ } split(/:/, $ENV{PATH}))')
}

function path_remove()  { export PATH=`echo -n $PATH | awk -v RS=: -v ORS=: '$0 != "'$1'"' | sed 's/:$//'`; }

red=$'\e[1;31m'
grn=$'\e[1;32m'
yel=$'\e[1;33m'
blu=$'\e[1;34m'
mag=$'\e[1;35m'
cyn=$'\e[1;36m'
end=$'\e[0m'
 
warning () {
    echo "${yel}WARNING:${end}: $@";
}

warn () {
    echo "${yel}WARNING:${end}: $@";
}
 
error () {
    echo "${red}ERROR:${end} $@" >&2;
    exit 99;
}
 
information () {
   echo "${grn}INFO:${end} $@";
}

info () { 
   echo "${grn}INFO:${end} $@"; 
}

debug () { 
   if [ ! -z "$DEBUG" ]; then 
     echo "${blu}DEBUG:${end} $@"; 
   fi
}

function try {
    debug "Trying command: $@"
    "$@"
    local status=$?
    if [ $status -ne 0 ]; then
        error "$@ failed with RC=${status}" 
    fi
    return $status
}

function get_socket_port {
  # Find a port to use 
  #
  UST_SW_LINK_usb_PORT=54321
  MAX_PORTS=128  
  for i in `seq 0 $MAX_PORTS`; do
      if ! netstat -nlt |grep ":$UST_SW_LINK_usb_PORT\b" > /dev/null; then
          break
      fi
      UST_SW_LINK_usb_PORT=$[$UST_SW_LINK_usb_PORT+1]
  done

  if [ $i == $MAX_PORTS ]; then
      error "All available ports for python script are in use";
  fi
  export UST_SW_LINK_usb_PORT=$UST_SW_LINK_usb_PORT
}
