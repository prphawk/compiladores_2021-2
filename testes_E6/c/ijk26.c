//208
int x;

int fatorial (int n)
{
    int temp;
    if (n == 1) {
       return n;
    };
    temp = fatorial(n-1) * n;
    return temp;          
}

int main()
{
  int y;
  y = 6;
  x = fatorial(y);
  return x;
}
