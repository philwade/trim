#include <stdio.h>

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
int trailing_white(int c)
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

		c = fgetc(stdin);
	}

}

int main(int argc, char **argv)
{
	FILE *input;
	int c;
	int last_char = '\n';
	int had_trail;

	input = stdin;

	while(EOF != (c = fgetc(input)))
	{
		if(c == ' ' || c == '\t')
		{
			if(last_char != '\n')
			{
				had_trail = trailing_white(c);
				if(had_trail)
				{
					last_char = '\n';
				}
				continue;
			}
		}
		else
		{
			fputc(c, stdout);
			last_char = c;
		}
	}
	return 0;
}
