#!/bin/bash

# Cores SHELL
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

COMPILER=./ec
INPUTFOLDER=./tests/in/
OUTPUTFOLDER=./tests/out/

# Create Temp Folder for outputs
mkdir -p temp

echo "BEGINING TESTS..."
echo
sleep 0.2

OK=0
FAIL=0

for f in $(find ${INPUTFOLDER} -name *.c); do
    inputFile=$(basename "${f}")
    outputName="${inputFile%.*}"
    outputFile="${outputName}.out"

    echo -ne ".......N${outputName}\r"
    sleep 0.05

    ${COMPILER} < $f > "./temp/${outputFile}" 1>&1 2>/dev/null

    result=$(diff -y --width=150 "${OUTPUTFOLDER}${outputFile}" "./temp/${outputFile}")

    if [ $? -eq 0 ]; then
        echo -e "${GREEN}PASS${NC}...N${outputName}"
        OK=$(($OK+1))
    else
        echo -e "${RED}FAIL${NC}...N${outputName}"
        echo
        echo -e "${YELLOW}${result}${NC}"
        echo
        FAIL=$(($FAIL+1))
    fi
done

if [ $FAIL -eq 0 ]; then
    echo
    echo -e "${GREEN}OK!${NC}"
else
    echo
    echo -e "${RED}FAIL!${NC} - SUCCESS RATIO=$((100*$OK/($OK+$FAIL)))%"
fi

# Delete Temp Folder
rm -rf temp