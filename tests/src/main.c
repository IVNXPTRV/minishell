/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ipetrov <ipetrov@student.42bangkok.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 01:39:55 by ipetrov           #+#    #+#             */
/*   Updated: 2025/01/29 02:39:38 by ipetrov          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <stdlib.h>  // For system()
#include <stdio.h>

int main()
{
	char *cmd = \
		"bash cat << EOF | cat > outfile";
    // Run a Bash command
    int result = system(cmd);

    // Check the result of the command
    if (result == -1)
	{
        // system() failed
        perror("system");
        return EXIT_FAILURE;
    } else if (result != 0)
	{
        // Command executed but returned a non-zero exit status
        printf("Command failed with exit code %d\n", result);
        return EXIT_FAILURE;
    }

    // Command executed successfully
    printf("Command executed successfully.\n");
    return EXIT_SUCCESS;
}
