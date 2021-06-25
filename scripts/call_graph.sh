# SPDX-FileCopyrightText: 2020 Anuradha Weeraman <anuradha@weeraman.com>
# SPDX-License-Identifier: GPL-3.0-or-later

echo $1
FILES=$(find . -name "*.expand" | tr '\n' ' ')
tools/egypt-1.10/egypt $FILES | dot -Grankdir=LR -Tsvg -o call_graph.svg
