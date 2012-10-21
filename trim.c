/*
 * trim - clean up whitespace
 * by Phil Wade <phil@philwade.org>
 * (c) 2012
 * MIT licensed: http://www.opensource.org/licenses/mit-license.php
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// options
static int all_whitespace;
static int only_start;
static int only_end;

void print_whitespace(int space_count, int tab_count)
{
	int i;
	for(i = 0; i < space_count; i++)
	{
		fputc(' ', stdout);
	}

	for(i = 0; i < tab_count; i++)
	{
		fputc('\t', stdout);
	}
}

// see if it is just white space until newline
// returns 1 if we have trailing whitespace
// 0 if not
int trailing_white(int c, FILE *stream)
{
	int space_counter = 0;
	int tab_counter = 0;

	while(1)
	{

		if(c == ' ')
		{
			// Still in whitespace land
			space_counter++;
		}
		else if(c == '\t')
		{
			tab_counter++;
		}
		else if(c == '\n')
		{
			// If we get here, we've hit newline before non-whitespace, so just bail.
			fputc(c, stdout);
			return 1;
		}
		else
		{
			// We just hit some mid line whitespace
			// Catch up on whitespace and print the character
			print_whitespace(space_counter, tab_counter);
			fputc(c, stdout);
			return 0;
		}

		c = fgetc(stream);
	}

}

void usage()
{
	fputs("\n\
trim - the whitespace cleaner. Removes trailing and beginning whitespace from stdin.\n\
\n\
Available flags: \n\
-a	Remove all the white space from your input. \n\
-s	Remove white space only from the start of your lines. \n\
-e	Remove white space only from the end of your lines. \n\
-h	This menu.\n\n\
", stdout);
	exit(0);
}

void trim_file(FILE *input)
{
	int c;
	int last_char = '\n';
	int had_trail;

	while(EOF != (c = fgetc(input)))
	{
		if(c == ' ' || c == '\t')
		{
			if(all_whitespace)
			{
				continue;
			}

			if(last_char != '\n')
			{
				if(!only_start)
				{
					had_trail = trailing_white(c, input);
					if(had_trail)
					{
						last_char = '\n';
					}
					continue;
				}
				else
				{
					fputc(c, stdout);
					last_char = c;
				}
			}
			else if(only_end)
			{
				fputc(c, stdout);
				last_char = c;
			}
		}
		else
		{
			fputc(c, stdout);
			last_char = c;
		}
	}
}

int main(int argc, char **argv)
{
	FILE *input;
	int o;

	while((o = getopt(argc, argv, "aseh")) != -1)
	{
		switch(o)
		{
			case 'a':
				all_whitespace = 1;
				break;
			case 's':
				only_start = 1;
				break;
			case 'e':
				only_end = 1;
				break;
			default:
				usage();
		}
	}

	if (optind == argc)
	{
		input = stdin;
		trim_file(stdin);
	}
	else
	{
		int i;
		for(i = optind;i < argc;i++)
		{
			input = fopen(argv[optind], "r");
			trim_file(input);
		}
	}

	return 0;
}
