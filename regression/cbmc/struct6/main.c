void *malloc(__CPROVER_size_t);
void free(void *);

struct S
{
  int x;
  char a[];
};

int main()
{
  struct S *p=malloc(sizeof(struct S)+10);
  
  p->x=1;
  p->a[0]=3;
  p->a[9]=3;

  free(p);
}
