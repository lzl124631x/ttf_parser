#ifndef MEM_STREAM_H
#define MEM_STREAM_H

namespace ttf_dll{
  class Mem_Stream{
  private:
    char *start;
    char *cursor;
    char *end;
  public:
    Mem_Stream(void *m, size_t size)
      : start((char*)m), cursor((char*)m), end((char*)m + size){}
    ~Mem_Stream(){
      start = cursor = end = NULL;
    }
    void read(void *buf, unsigned size);
    void seek(int offset);
    int tell();
    void *get_cursor();
    void read_big_endian(void *buf, unsigned size);
    void read_big_endian(void *buf, unsigned unit_size, unsigned unit_num);
  };

#define MREAD(s, b) s.read_big_endian((b), sizeof(*(b)))
#define MREAD_N(s, b, n) s.read_big_endian((b), sizeof(*(b)), (n))
}
#endif