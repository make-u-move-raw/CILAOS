int sum_until(int n){
    int res = 0;

    for(int i=0; i<n; i++)
        res+=i+1;

    return res;
}

int sum_until_rec(int n){
    return n <= 0 ? 0 : n + sum_until_rec(n-1); 
}