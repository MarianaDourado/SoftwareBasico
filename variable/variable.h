#pragma once

void processVariableDeclarations(char *line);
void processVariableDeclarationsEnd(char *line);
void processArrayDeclaration(char *line);
void processStackVariableDeclaration(char *line);
void processRegisterVariableDeclaration(char *line);
void saveRegisters();
void retrieveRegisters();
void processCallParameterAssignment(char *parameter, int parameterIndex);
char *processArithmeticOperator(char operator);
void processVariableAssignment(char *line);