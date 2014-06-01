#ifndef MEM_STREAM_H
#define MEM_STREAM_H

namespace ttf_dll {

class DLL_API MemStream {
 public:
  MemStream(void *m, size_t size)
    : start_((char*)m), cursor_((char*)m), end_((char*)m + size) {}
  ~MemStream() {
    start_ = cursor_ = end_ = NULL;
  }
  void Read(void *buf, unsigned size);
  void Seek(int offset);
  int Tell();
  void *cursor() const {
    return cursor_;
  }
  void ReadBigEndian(void *buf, unsigned size);
  void ReadBigEndian(void *buf, unsigned unit_size, unsigned unit_num);

 private:
  char *start_;
  char *cursor_;
  char *end_;
};

#define MREAD(s, b) s.ReadBigEndian((b), sizeof(*(b)))
#define MREAD_N(s, b, n) s.ReadBigEndian((b), sizeof(*(b)), (n))

} // namespace ttf_dll
#endif