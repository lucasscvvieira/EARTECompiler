int b, i, n;
char a, aux;

void main() { 
    // Assign initial values to the variables
    a = 0; 
    b = 1;
    // This block generates a sequence of numbers
    for(i = 0; a < n; i+=1) { 
	    aux = a + i; 
	    a = b; 
	    b = aux; 
    }
}