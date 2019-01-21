int main (int a) {
  double * foo = 0;
  if (a) {
    double * foo = malloc(sizeof(double) * 5);
  } else {
    double * foo = malloc(sizeof(double) * 10);
  }
  return sizeof(foo);
}
