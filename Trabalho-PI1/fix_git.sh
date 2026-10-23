#!/bin/bash
export GIT_SEQUENCE_EDITOR="sed -i -e 's/merge -C eb3356c/merge -c eb3356c/'"
export GIT_EDITOR="sed -i -e '/Co-authored-by: Cursor/d'"
git rebase -i --rebase-merges eb3356c^1
