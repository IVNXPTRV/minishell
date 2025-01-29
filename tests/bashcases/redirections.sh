# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    redirections.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/29 03:10:10 by ipetrov           #+#    #+#              #
#    Updated: 2025/01/29 06:27:12 by ipetrov          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#!/bin/bash

# Define color variables
RED="\e[31m"
GREEN="\e[32m"
BLUE="\e[34m"
BOLD="\e[1m"
YELLOW="\033[0;33m"
GREY="\033[38;5;244m"
PURPLE="\033[0;35m"
END="\033[0m"
RESET="\e[0m"

INDIR="./infiles"
	INBIG="inbig"
	INEMPTY="inempty"
	INORDINARY="inordinary"
	INPERM="inperm"
	INSPACES="inspaces"

OUTDIR="./outfiles"
	OUTORDINARY="outordinary"
	OUTPERM="outperm"

rm -rf "$INDIR" "$OUTDIR"

mkdir -p $INDIR $OUTDIR

init_testfiles() {
	local content

	head -c 750K /dev/urandom | tr -dc 'A-Za-z0-9 \n' > $INDIR/$INBIG

	content="line1\nline2\nline3"
	echo -e $content > $INDIR/$INORDINARY

	touch  $INDIR/$INEMPTY

	content="inperm content"
	echo -e $content > $INDIR/$INPERM
	chmod 000 $INDIR/$INPERM

	content="😈 😈 😈\nThis will break your minishell\n😈 😈 😈"
	echo -e $content > $INDIR/$INSPACES

	content="initially line1\ninitially line2\ninitially line3"
	echo -e $content > $OUTDIR/$OUTORDINARY

	content="outperm content"
	echo -e $content > $OUTDIR/$OUTPERM
	chmod 000 $OUTDIR/$OUTPERM

}

is_created() {
	local pathname="$1"

	echo -e "${GREY}Files:${RESET}"
	if [ -e $pathname ]; then
		echo -e "${GREY}$pathname has been created.${RESET}"
	else
		echo -e "${GREY}$pathname has NOT been created.${RESET}"
	fi | sed 's/^/\t/'
}

execute() {
    local brief="$1"
    local prompt="$2"
	local exitcode
	local output

	init_testfiles
    echo -e "\n"
	echo -e "$(printf '%*s' $(tput cols) | tr ' ' -)"
	echo -e "\n"
	output=$(eval "$prompt" 2>&1)
	exitcode=$?
    echo -e "${GREY}Brief:${RESET} $brief"
    echo -e "${GREY}Prompt:${RESET} ${YELLOW}$prompt${RESET}"
	echo -e "${GREY}Code:${RESET} $exitcode"
    echo -e "${GREY}Output:${RESET}"
    echo "$output" | sed 's/^[^:]*:[ ]*line [0-9]*: //' | sed 's/^/\t/'
}

execute \
	"Ordinary command" \
	"ls | cat" \

execute \
	"No permissions to infile" \
	"< infiles/inperm cat" \

execute \
	"No permissions to outfile" \
	"ls > outfiles/outperm" \

execute \
	"No permissions to infile and outfile" \
	"< infiles/inperm cat > outfiles/outperm" \

execute \
	"No permissions to infile while valid out redir" \
	"< infiles/inperm cat > outfiles/outordinary" \

execute \
	"No permissions to infile while valid out redir to non existing file" \
	"< infiles/inperm cat > outfiles/outnonexist" \

is_created "outfiles/outnonexist"

execute \
	"All permissions to infile and outfile" \
	"< infiles/inordinary cat > outfiles/outordinary" \

execute \
	"All permissions to infile and outfile, not valid command" \
	"< infiles/inordinary caaat > outfiles/outordinary" \

execute \
	"No permissions to infile and outfile, not valid command" \
	"< infiles/inperm caaat > outfiles/outperm" \

execute \
	"No permissions to infile, all permissions to outfile, not valid command" \
	"< infiles/inperm caaat > outfiles/outordinary" \

execute \
	"All permissions to infile, no permissions to outfile, not valid command" \
	"< infiles/inordinary caaat > outfiles/outperm" \

execute \
	"All permissions to outfile1, no permissions to outfile2, not valid command" \
	"caaat > outfiles/outordinary > outfiles/outperm" \

execute \
	"All permissions to outfile1, no permissions to outfile2, valid command" \
	"ls / > outfiles/outordinary > outfiles/outperm" \

execute \
	"Nonexisting outfile1, no permissions to outfile2, valid command" \
	"ls / > outfiles/outnonexist > outfiles/outperm" \

is_created "outfiles/outnonexist"

execute \
	"No permissions infile, Nonexisting outfile1, no permissions to outfile2, valid command" \
	"< infiles/inperm ls / > outfiles/outnonexist > outfiles/outperm" \

is_created "outfiles/outnonexist"

execute \
	"All permissions infile, No permissions to outfile1, Nonexisting outfile1, valid command" \
	"< infiles/inordinary cat > outfiles/outperm > outfiles/outnonexist" \

is_created "outfiles/outnonexist"

echo $USER

execute \
	"Test VAR" \
	'echo *' \

echo -e "\n"