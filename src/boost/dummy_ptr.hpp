namespace boost
{
  template<class T> 
  class dummy_ptr
  {
  private:
    T* data;
  public:
    dummy_ptr () { data = 0; }
    dummy_ptr (T* d) : data (d) {}
    dummy_ptr (const dummy_ptr& d) : data (d.data) {}
    T* get () { return data; }
  };
}

// EOF //
