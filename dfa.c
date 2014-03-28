/* dfa: Run and visualise DFAs defined by the DOT language. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <err.h>
#include <getopt.h>
#include <stdbool.h>
#include "graphviz/cgraph.h"
#include "graphviz/gvc.h"

void init(Agraph_t *g);
void vis(Agraph_t *g);
Agnode_t *delta(Agraph_t *g, Agnode_t *n, char c);
void run(Agraph_t *g);

/* Try to find the delta transition for a given state and character. Returns
 * the next state if found, else returns NULL. */
Agnode_t *delta(Agraph_t *g, Agnode_t *n, char c)
{
	Agedge_t *e;

	for (e = agfstout(g, n); e; e = agnxtout(g, e))
		if (strchr(agget(e, "label"), c))
			return e->node;
	return NULL;
}

/* Run the DFA across strings provided from stdin. */
void run(Agraph_t *g)
{
	int len;
	char c, str[1024], *alphabet;
	Agnode_t *n, *m, *fst;

	alphabet = agget(g, "alphabet");
	if (!alphabet)
		errx(EX_DATAERR, "error: no alphabet");
	printf("alphabet = {%s}\n", alphabet);

	fst = agnode(g, "start", FALSE);
	if (fst)
		n = agfstout(g, fst)->node;
	else
		errx(EX_DATAERR, "error: no start node");

	/* Continously prompt the user for strings to run the DFA with until
	 * `exit' is provided or the end-of-file is reached. */
	while (printf("> "), scanf("%s", str) != EOF) {
		if (strcmp(str, "exit") == 0)
			exit(0);
		len = strlen(str);

		/* Check that all characters in the provided string are from
		 * the given alphabet. */
		alphabet = agget(g, "alphabet");
		for (int i = 0; i < len; i++)
			if (!strchr(alphabet, str[i])) {
				warnx("warning: %c isn't in the alphabet",
						str[i]);
				goto input_warning;
			}

		/* Run the DFA across the string. */
		for (int i = 0; i < len; i++) {
			m = delta(g, n, str[i]);

			/* There was no delta transition, warn the user and
			 * prompt for another string. */
			if (!m) {
				warnx("warning: no delta(%s, %c)", agnameof(n),
						str[i]);
				goto input_warning;
			}

			/* Scanned intput */
			for (int j = 0; j < i; j++)
				putchar(str[j]);

			/* Transition; it isn't safe to use `agnameof' twice in
			 * the same statement as a temporary buffer is used
			 * between calls. */
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

input_warning:
		n = agfstout(g, fst)->node;
	}
}

/* Initialise attributes for the different types of nodes to reduce the
 * verboseness of the DFA definition files. */
void init(Agraph_t *g)
{
	Agnode_t *n;

	/* Make the following attributes available and provide a default. */
	agattr(g, AGRAPH, "rankdir", "LR");
	agattr(g, AGNODE, "shape", "circle");
	agattr(g, AGNODE, "width", ".5");
	agattr(g, AGNODE, "height", ".5");
	agattr(g, AGNODE, "label", "");

	/* Set the label for all nodes except the `start' node. */
	for (n = agfstnode(g); n; n = agnxtnode(g, n))
		if (strcmp("start", agnameof(n)) != 0)
			agset(n, "label", agnameof(n));

	/* The accept states is a comma and/or space seperated set of states. */
	char *sep = ", ";
	char *accept = agget(g, "accept");
	if (!accept)
		errx(EX_DATAERR, "error: no accept states specified");

	/* Make the final states' shape a double circle. */
	for (char *q = strtok(accept, sep); q; q = strtok(NULL, sep)) {
		n = agnode(g, q, FALSE);
		if (!n)
			errx(EX_DATAERR, "error: no node %s from final states",
					q);
		agset(n, "shape", "doublecircle");
	}

	/* Set start state attributes; the node shouldn't be displayed. */
	n = agnode(g, "start", FALSE);
	if (!n)
		errx(EX_DATAERR, "error: no start start");
	agset(n, "shape", "none");
	agset(n, "width", "0");
	agset(n, "height", "0");
}

/* Draw the graph as an SVG using the DOT layout engine. */
void vis(Agraph_t *g)
{
	GVC_t *gvc = gvContext();
	gvLayout(gvc, g, "dot");
	gvRender(gvc, g, "svg", stdout);
	gvFreeLayout(gvc, g);
	gvFreeContext(gvc);
}

int main(int argc, char *argv[])
{
	Agraph_t *g;
	FILE *fd;
	char ch;
	bool v;

	while ((ch = getopt(argc, argv, "tv?")) != -1) {
		switch (ch) {
		case 'v':
			v = true;
			break;
		/*case 't':
			t = true;
			break;
		case '?':
			usage();*/
		}
	}

	if (argv[optind])
		fd = fopen(argv[optind], "r");
	else
		errx(EX_NOINPUT, "error: no input file");
	g = agread(fd, NULL);
	fclose(fd);

	init(g);
	if (v)
		vis(g);
	else
		run(g);

	agclose(g);
	return 0;
}
