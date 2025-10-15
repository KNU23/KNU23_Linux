# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main(int argc, char *argv[]){

	double num1 = atof(argv[1]);
	char op = argv[2][0];
	double num2 = atof(argv[3]);

	double result = 0.0;

	switch (op) {
		case '+':
			result = num1 + num2;
			break;
		case '-':
			result = num1 - num2;
			break;
		case 'x':
			result = num1 * num2;
			break;
		case '/':
			if (num2 == 0) {
				return 1;
			}
			result - num1 / num2;
			break;
		default:
			return 1;
	}

	printf("%g\n", result);

	return 0;
}
