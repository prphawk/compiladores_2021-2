//7
int K;

int gis(int a)
{
   int ret;
   int b;
   if (a == 0) { return 1; };
   b = a - 1;
   ret = a + gis(b);
   return ret;
}

int main()
{
  int a;
  a = gis(3);
  return a;
}
