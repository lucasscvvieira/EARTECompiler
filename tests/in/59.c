int a, b, aux, i, n;

void main() { 
    // Assign initial values to the variables
    a = 0; 
    b = 1;
    // This block generates a sequence of numbers
    for(i = 0; i < n; i=i+1) { 
        for(j=0;j<i;j=j+1){
            b = b + j;
        }
	    aux = a + b; 
	    a = b; 
	    b = aux; 
    }
}