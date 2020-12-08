int a, b, aux, i, n;

int main() { 
    // Assign initial values to the variables
    a = 0; 
    b = 1;
    // This block generates a sequence of numbers
    for(i = 0; (i < n) & i < 100 ; i=i+1) { 
	    aux = a + b; 
	    a = b; 
	    b = aux; 
    }

}