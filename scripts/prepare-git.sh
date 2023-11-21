#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Add git prepare-commit-msg hook.
ln -s ../../scripts/prepare-commit-msg.sh ${SCRIPT_DIR}/../.git/hooks/prepare-commit-msg
chmod +x ${SCRIPT_DIR}/prepare-commit-msg.sh
