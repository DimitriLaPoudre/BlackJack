#!/bin/bash

# Global variables
BINARY=$1
TEST=$(dirname "${BASH_SOURCE[0]}")/test
TEST_CODE=$2
TRAPSIG=0
EXIT_CODE=0

# MACRO executable
CAT=`which cat`
GREP=`which grep`
SED=`which sed`
RM=`which rm`
TR=`which tr`
HEAD=`which head`
TAIL=`which tail`
WC=`which wc`
CHMOD=`which chmod`
EXPR=`which expr`
MKDIR=`which mkdir`
CP=`which cp`

# Check error conditions
if [ -z "$BINARY" ] || [ -z "$TEST" ]; then
  echo "Usage: $0 <binary> [test_id]"
  exit 1
fi
if [ ! -x "$BINARY" ]; then
  echo "Error: $BINARY is not an executable file."
  echo "Usage: $0 <binary> [test_id]"
  exit 1
fi
if [ ! -f "$TEST" ]; then
  echo "Error: $TEST is not a file."
  echo "Usage: $0 <binary> [test_id]"
  exit 1
fi
if [ ! -r "$TEST" ]; then
  echo "Error: $TEST is not readable."
  echo "Usage: $0 <binary> [test_id]"
  exit 1
fi

# Prepare the test script
prepare_test()
{
  local outputfn="/tmp/.output.$$"
  local runnerfn="/tmp/.runner.$$"

  WRAPPER="$runnerfn"

  echo "#!/bin/bash" > $runnerfn
  echo "$SETUP" >> $runnerfn
  echo "echo -n $INPUT | $TR '²' '\n' | ./$BINARY $ARGS > $outputfn 2>&1" >> $runnerfn
  echo "ret=\$?" >> $runnerfn
  echo "$CLEAN" >> $runnerfn
  echo "exit \$ret" >> $runnerfn

  chmod 755 $runnerfn
}

# Load the test
load_test()
{
  local id=$1
  local test=$($SED -n "/\[$id\]/,/\[$id-END\]/p" "$TEST" | $SED "1d;\$d")

  NAME=`echo "$test" | $GREP "NAME=" | $SED s/'NAME='//`
  DESC=`echo "$test" | $GREP "DESC=" | $SED s/'DESC='//`
  SETUP=`echo "$test" | $GREP "SETUP=" | $SED s/'SETUP='//`
  CLEAN=`echo "$test" | $GREP "CLEAN=" | $SED s/'CLEAN='//`
  ARGS=`echo "$test" | $GREP "ARGS=" | $SED s/'ARGS='//`

  INPUT=`echo -n "$test" | $SED -n "/INPUT=/,/OUTPUT=/p" | $GREP -v -e "INPUT=" -e "OUTPUT=" | $SED -z 's/.$//' | $TR "\n" "²"`
  echo -n "$test" | $SED -n "/OUTPUT=/,/CODE=/p" | $GREP -v -e "OUTPUT=" -e "CODE=" | $SED -z 's/.$//' > /tmp/.ref.$$

  CODE=`echo "$test" | $GREP "CODE=" | $SED s/'CODE='//`

  if [ -z "$CODE" ]
  then
    return
  fi
  
  prepare_test
  $WRAPPER 2>&1 > /dev/null

  if [ $? -ne $CODE ]
  then
    echo "Test $id ($NAME) : KO - Code $CODE expected, but got $?"
    return
  fi
  
  ok=1
  i=1

  exec 3< /tmp/.ref.$$
  exec 4< /tmp/.output.$$

  while true; do
    read -r line_ref <&3 || break
    read -r line_out <&4 || break

    if [ "$line_ref" != "$line_out" ]; then
      ok=0
      break;
    fi
    i=$((i + 1))
  done

  read -r _ <&3 && ok=0
  read -r _ <&4 && ok=0


  if [ $ok -eq 1 ]
  then
      echo "Test $id ($NAME) : OK"
  else
      EXIT_CODE=1
      echo "Test $id ($NAME) : KO - Check output in /tmp/test.$$/$id/" 
      echo "Outputs differs at line: $i"
      $MKDIR -p /tmp/test.$$/$id 2>/dev/null
      $CP /tmp/.output.$$ /tmp/test.$$/$id/output.out
      $CP /tmp/.ref.$$ /tmp/test.$$/$id/excepted.out
  fi
}


if [ $TRAPSIG -eq 1 ]
then
  for sig in `trap -l`
  do
    echo "$sig" | grep "^SIG" >/dev/null 2>&1
    if [ $? -eq 0 ]
    then
      trap "echo Received signal $sig !" $sig
    fi
  done
fi

if [ -z "$TEST_CODE" ]
then
  for lst in `cat "$TEST" | grep "^\[.*\]$" | grep -vi end | sed s/'\['// | sed s/'\]'//`
  do
    load_test $lst
  done
else
    load_test $TEST_CODE
fi

if [ $EXIT_CODE -ne 0 ]
then
  exit 1
else
  exit 0
fi