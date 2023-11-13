#!/bin/bash

# Add git prepare-commit-msg hook.
ln -s prepare-commit-msg.sh ../.git/hooks/prepare-commit-msg
chmod +x prepare-commit-msg.sh
