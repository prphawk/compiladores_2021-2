//115
int x;

int f(int z, int w)
{
  int temp;
  temp = z*w;
  return temp;
}

int mult (int z, int w, int a, int b)
{
  int m;
  int n;
  int o;
  int p;
  int r;
  int s;
  int t;
  m = z;
  m = m + 1;
  n = w;
  n = n + 3;
  o = a;
  o = o + 2;
  p = b;
  p = p + 4;
  r = f(m,n);
  s = f(o,p);
  t = r + s;
  return t;
}

int main()
{
  int y;
  int z;
  y = 7;
  z = 3;
  x = mult(y, y, z, z);
  return x;
}
