//43
int a;
int f()
{
  int x;
  x = 99;
  return x;
}

int main() {
    int c;
    c = 456;
    a = f();
    a = a + c;
    return a;
}