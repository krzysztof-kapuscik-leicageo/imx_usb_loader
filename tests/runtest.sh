#!/bin/sh

set -x

echo PP=$*

EXENAME=$1
TESTNAME=$2
CONFIGDIR=$3

rm -f ${TESTNAME}.result
rm -f ${TESTNAME}.result.filtered
rm -f ${TESTNAME}.output.filtered

${EXENAME} -c ${CONFIGDIR} -v -S 0x15a2:0x0061 ${TESTNAME} > ${TESTNAME}.result
echo EXERESULT=$?

grep -v -E "config file |parse |global configuration directory:" \
    ${TESTNAME}.result > ${TESTNAME}.result.filtered
grep -v -E "config file |parse |global configuration directory:" \
    ${TESTNAME}.output > ${TESTNAME}.output.filtered

diff -s ${TESTNAME}.result.filtered ${TESTNAME}.output.filtered
RESULT=$?
echo DIFFRESULT=${RESULT}

exit ${RESULT}
