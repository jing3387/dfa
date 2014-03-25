#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sysexits.h>
#include <err.h>
#include "graphviz/cgraph.h"

Agnode_t *delta(Agraph_t *g, Agnode_t *n, char c)
{
	Agedge_t *e;
	for (e = agfstout(g, n); e; e = agnxtout(g, e))
		if (strchr(agget(e, "label"), c))
			return e->node;
	return NULL;
}

void run(Agraph_t *g)
{
	int len;
	char c, str[1024];
	Agnode_t *n, *m, *fst;
	if (!agget(g, "Sigma"))
		errx(EX_DATAERR, "no alphabet");
	printf("Sigma = {%s}\n", agget(g, "Sigma"));
	fst = agnode(g, "start", FALSE);
	if (fst)
		n = agfstout(g, fst)->node;
	else
		errx(EX_DATAERR, "no start node");
	while (printf("> "), scanf("%s", str) != EOF) {
		if (strcmp(str, "exit") == 0)
			exit(0);
		len = strlen(str);
		for (int i = 0; i < len; i++) {
			m = delta(g, n, str[i]);
			/* There was no delta transition, warn the user and
			 * prompt for another string. */
			if (!m) {
				warnx("no delta(%s, %c)", agnameof(n),
						str[i]);
				goto nodelta;
			}
			/* Scanned intput */
			for (int j = 0; j < i; j++)
				putchar(str[j]);
			/* Transition */
			printf("\t%s -- %c --> ", agnameof(n), str[i]);
			printf("%s\t", agnameof(m));
			/* Unscanned input */
			for (int j = i + 1; j < len; j++)
				putchar(str[j]);
			putchar('\n');
			n = m;
		}
		if (!strcmp(agget(n, "shape"), "doublecircle"))
			printf("Accepted\n");
		else
			printf("Declined\n");
nodelta:
		n = agfstout(g, fst)->node;
	}
}

int main(int argc, char **argv)
{
	Agraph_t *g;
	FILE *fd;
	char ch;
	if (argv[1])
		fd = fopen(argv[1], "r");
	else
		errx(EX_NOINPUT, "error: no input file");
	g = agread(fd, NULL);
	fclose(fd);
	run(g);
	agclose(g);
	return 0;
}
