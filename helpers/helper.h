#pragma once

#define boolean int

void removeNewline(char *line);
void removeComments(char *line);
void printLine(char *line, int count);
int contains(char **source, char *target);
char *replace(char *source, char *target, char *replacement);
boolean startsWith(char *line, char *keyword);
