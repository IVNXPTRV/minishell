# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    redirections.sh                                    :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/29 03:10:10 by ipetrov           #+#    #+#              #
#    Updated: 2025/01/29 08:25:31 by ipetrov          ###   ########.fr        #
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

# is_created() {
# 	local pathname="$1"

# 	echo -e "${GREY}Files:${RESET}"
# 	if [ -e $pathname ]; then
# 		echo -e "${GREY}$pathname has been created.${RESET}"
# 	else
# 		echo -e "${GREY}$pathname has NOT been created.${RESET}"
# 	fi | sed 's/^/\t/'
# }

execute() {
    local brief="$1"
    local prompt="$2"
	local exitcode

	declare -A before_in=()
	declare -A after_in=()

	declare -A before_out=()
	declare -A after_out=()

	init_testfiles
	for file in $INDIR/*; do
		stat_output=$(stat "$file")
		before_in["$file"]="$stat_output"
	done

	for file in $OUTDIR/*; do
		stat_output=$(stat "$file")
		before_out["$file"]="$stat_output"
	done

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

	for file in $INDIR/*; do
		stat_output=$(stat "$file")
		after_in["$file"]="$stat_output"
	done

	for file in $OUTDIR/*; do
		stat_output=$(stat "$file")
		after_out["$file"]="$stat_output"
	done

	all_keys_in=()
	for key in "${!before_in[@]}"; do
		all_keys_in+=("$key")
	done
	for key in "${!after_in[@]}"; do
		all_keys_in+=("$key")
	done
	all_keys_in=($(printf "%s\n" "${all_keys_in[@]}" | sort | uniq))

	echo -e "${GREY}Infiles:${RESET}"
	for key in "${all_keys_in[@]}"; do
		diff_output=$(diff <(echo "${before_in[$key]}") <(echo "${after_in[$key]}"))
		if [[ -n "$diff_output" ]]; then  # Check if diff output is NOT empty
				echo -e "${RED} $(echo "${after_in[$key]}"| grep "File:" | awk '{print $2}') ${RESET}"
				echo "$diff_output" | sed -n 's/^> //p' | sed 's/^/\t/'
				echo -e "\n"
		fi
	done

	all_keys_out=()
	for key in "${!before_out[@]}"; do
		all_keys_out+=("$key")
	done
	for key in "${!after_out[@]}"; do
		all_keys_out+=("$key")
	done
	all_keys_out=($(printf "%s\n" "${all_keys_out[@]}" | sort | uniq))

	echo -e "${GREY}Outfiles:${RESET}"
	for key in "${all_keys_out[@]}"; do
		diff_output=$(diff <(echo "${before_out[$key]}") <(echo "${after_out[$key]}"))
		if [[ -n "$diff_output" ]]; then  # Check if diff output is NOT empty
				echo -e "${RED} $(echo "${after_out[$key]}"| grep "File:" | awk '{print $2}') ${RESET}"
				echo "$diff_output" | sed -n 's/^> //p' | sed 's/^/\t/'
				echo -e "\n"
		fi
	done
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


execute \
	"No permissions infile, Nonexisting outfile1, no permissions to outfile2, valid command" \
	"< infiles/inperm ls / > outfiles/outnonexist > outfiles/outperm" \


execute \
	"All permissions infile, No permissions to outfile1, Nonexisting outfile1, valid command" \
	"< infiles/inordinary cat > outfiles/outperm > outfiles/outnonexist" \


echo $USER

execute \
	"Test VAR" \
	'echo *' \

rm -rf "$INDIR" "$OUTDIR"
echo -e "\n"