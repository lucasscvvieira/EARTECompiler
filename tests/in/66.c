int a, b, aux, i, n;

void main() { 
    // Assign initial values to the variables
    a = i = 0; 
    b = 1;
    // This block generates a sequence of numbers
    for(; i < n; i=i+1) { 
        for(int j=0;j<i;j=j+1){
            break;
        }
	    aux = a + b; 
	    a = b; 
	    b /= aux; 
    }
}