unsigned char inb(unsigned short port)
{
  unsigned char ret_val;

  __asm__ __volatile__("inb %1,%0"
    : "=a"(ret_val)
    : "d"(port));
  return ret_val;
}

void outb(unsigned port, unsigned val)
{
  __asm__ __volatile__("outb %b0,%w1"
    :
    : "a"(val), "d"(port));
}