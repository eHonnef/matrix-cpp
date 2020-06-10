float determinant(float ** matrix, int size) {
    float det = 0;
    if(!matrix)
        return 0;
    if(size == 1) 
	det = matrix[0][0];
    else {
        float ** sub_matrix = new float*[size-1];
        for(int i = 0; i < size -1; ++i)
            sub_matrix[i] = new float[size-1];
        for(int i = 0; i < size; ++i) {
            for(int k = 1; k < size; ++k) {
                int b = 0;
                for(int j = 0; j < size; ++j) {
                    if(j == i)
     	 	        continue;
                    sub_matrix[k-1][b] = matrix[k][j];
 		    ++b;
                }
            }
            int sign;
            if(i % 2 == 0) 
                sign = 1;
            else
                sign = -1;
            det += sign * matrix[0][i] * determinant(sub_matrix, size-1);
        } 
    }
    return det;
}