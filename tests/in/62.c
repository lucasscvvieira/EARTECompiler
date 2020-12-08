int a, b, aux, i, n;

void main() { 
    // Assign initial values to the variables
    a = 0; 
    b = 1;
    void c(){
        printf("func void");
    }
    // This block generates a sequence of numbers
    for(i = 0; i < n; i+=1) { 
	    aux = a * c(); 
	    a = b; 
	    b = aux; 
    }
}